/*
 * file/git-file.c - Open and read a file from git version control system
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define	_GNU_SOURCE	/* for get_current_dir_name */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <git2.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "file/file.h"
#include "file/git-util.h"
#include "file/git-file.h"


struct vcs_git {
	const char *name;
	const char *revision;
	const struct vcs_git *related;

	git_repository *repo;
	git_commit *commit;
	git_tree *tree;
	git_object *obj;

	const void *data;
	unsigned size;
};


/* ----- OID matching ------------------------------------------------------ */


void *vcs_git_get_oid(const void *ctx)
{
	const struct vcs_git *vcs_git = ctx;
	struct git_oid *new;

	new = alloc_type(git_oid);
	git_oid_cpy(new, git_object_id(vcs_git->obj));
	return new;
}


bool vcs_git_oid_eq(const void *a, const void *b)
{
	return !git_oid_cmp(a, b);
}


/* ----- Open -------------------------------------------------------------- */


static git_repository *select_repo(const char *path)
{
	git_repository *repo = NULL;
	char *tmp = stralloc(path);
	char *slash;

	/*
	 * If we can't find a repo, this may be due to the file or directory
	 * the path points to not existing in the currently checked-out tree.
	 * So we trim off elements until we find a repository.
	 *
	 * @@@ should we make relative paths absolute first ?
	 */
	while (1) {
		const char *try = *tmp ? tmp : *path == '/' ? "/" : ".";

		progress(3, "trying \"%s\"", try);
		if (!git_repository_open_ext_caching(&repo, try,
		    GIT_REPOSITORY_OPEN_CROSS_FS, NULL))
			break;
		if (!*tmp)
			break;
		slash = strrchr(tmp, '/');
		if (!slash) {
			if (*path == '/')
				break;
			*tmp = 0;
			continue;	/* try "." */
		}
		*slash = 0;
	}
	free(tmp);
	return repo;
}


static git_commit *pick_revision(git_repository *repo, const char *revision)
{
	git_object *obj;

	if (git_revparse_single(&obj, repo, revision))
		pfatal_git(git_repository_path(repo));

	if (git_object_type(obj) != GIT_OBJ_COMMIT)
		fatal("%s: not a commit", revision);

	return (git_commit *) obj;
}


static char *canonical_path_into_repo(const char *repo_dir, const char *path)
{
	struct stat repo_st, path_st;
	char *tmp, *tmp2, *slash, *tail, *real;
	char *to;
	const char *end, *from;

	/* identify inode of repo root */

	if (stat(repo_dir, &repo_st) < 0)
		diag_pfatal(repo_dir);
	if (!S_ISDIR(repo_st.st_mode))
		fatal("%s: not a directory\n", repo_dir);

	/* convert relative paths to absolute */

	if (*path == '/') {
		tmp = stralloc(path);
	} else {
		char *cwd = get_current_dir_name();

		tmp = alloc_size(strlen(cwd) + 1 + strlen(path) + 1);
		sprintf(tmp, "%s/%s", cwd, path);
		free(cwd);
	}

	/* remove trailing / */

	slash = strrchr(tmp, '/');
	if (slash && slash != tmp && !slash[1])
		*slash = 0;

	/*
	 * If path does point to inexistent object, separate into the part that
	 * is valid on the current system and the tail containing dead things.
	 */
	end = tail = strchr(tmp, 0);

	while (1) {
		progress(3, "probing \"%s\" tail \"%s\"", tmp, tail);
		if (stat(tmp, &path_st) == 0)
			break;
		if (!tmp[1])
			fatal("%s: cannot resolve\n", path);
		slash = strrchr(tmp, '/');
		if (tail != end)
			tail[-1] = '/';
		tail = slash + 1;
		*slash = 0;
	}

	/* remove . and .. from tail */

	progress(3, "input tail \"%s\"", tail);
	from = to = tail;
	while (1) {
		if (!strncmp(from, "./", 2)) {
			from += 2;
			continue;
		}
		if (!strcmp(from, "."))
			break;
		if (strncmp(from, "../", 3) && strcmp(from, "..")) {
			while (*from) {
				*to++ = *from++;
				if (from[-1] == '/')
					break;
			}
			if (!*from)
				break;
		}

		/*
		 * We have something like this:
		 * /home/repo/dead/../../foo
		 */
		if (to == tail)
			fatal("%s: can't climb out of dead path\n", path);

		/*
		 * We have something like
		 * "foo/" -> ""
		 * or
		 * "foo/bar/" -> "foo/"
		 * where "to" points to the end.
		 */
		to--;
		while (to != tail && to[-1] != '/')
			to--;
	}
	*to = 0;
	progress(3, "output tail \"%s\"", tail);

	/* resolve all symlinks */

	real = realpath(tmp, NULL);
	progress(3, "realpath(\"%s\") = \"%s\"", tmp, real);

	/* append tail */

	if (*tail) {
		tmp2 = alloc_size(strlen(real) + 1 + strlen(tail) + 1);
		sprintf(tmp2, "%s/%s", real, tail);
		free(real);
	} else {
		tmp2 = real;
	}
	free(tmp);
	tmp = tmp2;

	progress(2, "full object path \"%s\"", tmp);

	/*
	 * Re-validate path. If this fails, just give up and leave it to
	 * caller to implement fallbacks to save the day.
	 *
	 * @@@ we should also check if we've ended up in a different repo
	 * @@@ we could try to search again, with the new path. Crossing
	 *     from repo back to file could get "interesting".
	 */

	if (!select_repo(tmp)) {
		error("%s: outside repository", tmp);
		return NULL;
	}

	/* find which part of our path is inside the repo */

	end = tail = strchr(tmp, 0);
	while (1) {
		progress(3, "trying \"%s\" tail \"%s\"", tmp, tail);

		if (stat(tmp, &path_st) == 0 &&
		    path_st.st_dev == repo_st.st_dev &&
		    path_st.st_ino == repo_st.st_ino)
			break;

		slash = strrchr(tmp, '/');

		/* "this cannot happen" */
		if (tail == tmp || !slash)
			fatal("divergent paths:\nrepo \"%s\"\nobject \"%s\"",
			    repo_dir, tail);

		if (tail != end)
			tail[-1] = '/';
		tail = slash + 1;
		*slash = 0;
	}

	progress(2, "path in repo \"%s\"", tail);

	tmp2 = stralloc(tail);
	free(tmp);
	return tmp2;
}


static git_tree_entry *find_file(git_repository *repo, git_tree *tree,
    const char *path)
{
	git_tree_entry *entry;
	char *repo_path = stralloc(git_repository_workdir(repo));
		/* use workdir, not path, for submodules */
	char *slash, *canon_path;
	int len;

	/* remove trailing / from repo_path */
	slash = strrchr(repo_path, '/');
	if (slash && slash != repo_path && !slash[1])
		*slash = 0;

	len = strlen(repo_path);
	if (len >= 5 && !strcmp(repo_path + len - 5, "/.git"))
		repo_path[len == 5 ? 1 : len - 5] = 0;

	progress(2, "repo dir \"%s\"", repo_path);

	canon_path = canonical_path_into_repo(repo_path, path);
	free(repo_path);
	if (!canon_path)
		return NULL;

	if (git_tree_entry_bypath(&entry, tree, canon_path)) {
		perror_git(path);
		free(canon_path);
		return NULL;
	}
	free(canon_path);

	return entry;
}


static const void *get_data(struct vcs_git *vcs_git, git_tree_entry *entry,
    unsigned *size)
{
	git_repository *repo =vcs_git->repo;
	git_object *obj;
	git_blob *blob;

	if (git_tree_entry_type(entry) != GIT_OBJ_BLOB)
		fatal("entry is not a blob\n");
	if (git_tree_entry_to_object(&obj, repo, entry))
		pfatal_git("git_tree_entry_to_object");
	vcs_git->obj = obj;

	if (verbose > 2) {
		git_buf buf = { 0 };

		if (git_object_short_id(&buf, obj))
			pfatal_git("git_object_short_id");
		progress(3, "object %s", buf.ptr);
		git_buf_free(&buf);
	}
	blob = (git_blob *) obj;
	*size = git_blob_rawsize(blob);
	return git_blob_rawcontent(blob);
}


static bool send_line(const char *s, unsigned len,
    bool (*parse)(const struct file *file, void *user, const char *line),
    void *user, const struct file *file)
{
	char *tmp = alloc_size(len + 1);
	bool res;

	memcpy(tmp, s, len);
	tmp[len] = 0;
	res = parse(file, user, tmp);
	free(tmp);
	return res;
}


static bool access_file_data(struct vcs_git *vcs_git, const char *name)
{
	git_tree_entry *entry;

	entry = find_file(vcs_git->repo, vcs_git->tree, name);
	if (!entry)
		return 0;
	progress(1, "reading %s", name);

	vcs_git->data = get_data(vcs_git, entry, &vcs_git->size);
	return 1;
}


static bool related_same_repo(struct vcs_git *vcs_git)
{
	const struct vcs_git *related = vcs_git->related;

	vcs_git->repo = related->repo;
	vcs_git->commit = related->commit;
	vcs_git->tree = related->tree;

	return access_file_data(vcs_git, vcs_git->name);
}


static bool related_other_repo(struct vcs_git *vcs_git)
{
	static bool shown = 0;

	/* @@@ find revision <= date of revision in related */
	if (!shown)
		warning("related_other_repo is not yet implemented");
	shown = 1;
	return 0;
}


static bool related_only_repo(struct vcs_git *vcs_git)
{
	const struct vcs_git *related = vcs_git->related;
	char *tmp;

	progress(2, "trying graft \"%s\" \"%s\"",
	    related->name, vcs_git->name);
	tmp = file_graft_relative(related->name, vcs_git->name);
	if (!tmp)
		return 0;

	/*
	 * We now have a new path, but where does it lead ? If it contains a
	 * symlink, we may end up in an entirely different repo, where new
	 * adventures await. Let's find out ...
	 */
	vcs_git->repo = select_repo(tmp);
	if (vcs_git->repo) {
		free((char *) vcs_git->name);
		vcs_git->name = tmp;
		if (!strcmp(git_repository_path(vcs_git->related->repo),
		    git_repository_path(vcs_git->repo)))
			return related_same_repo(vcs_git);
		else
			return related_other_repo(vcs_git);
	}

	vcs_git->repo = related->repo;
	vcs_git->commit = related->commit;
	vcs_git->tree = related->tree;

	if (!access_file_data(vcs_git, tmp)) {
		free(tmp);
		return 0;
	}

	free((char *) vcs_git->name);
	vcs_git->name = tmp;

	return 1;
}


static bool try_related(struct vcs_git *vcs_git)
{
	if (!vcs_git->related)
		return 0;
	if (vcs_git->revision)
		return 0;

	progress(3, "try_related: %s %s",
	    vcs_git->related->name, vcs_git->related->revision);
	vcs_git->repo = select_repo(vcs_git->name);
	if (vcs_git->repo) {
		if (!strcmp(git_repository_path(vcs_git->related->repo),
		    git_repository_path(vcs_git->repo)))
/*
 * @@@ cd neo900/ee/hw; eeshow neo900.pro  works but
 * cd neo900/ee; eeshow hw/neo900.pro  fails because eeshow tries to find
 * files like bbcpu.sch in ee/ instead of ee/hw/.
 *
 * Changing the #if below to 0 makes this work in both cases but may produce
 * other issues. One of them is that the path of the related VCS file becomes
 * hw/hw/neo900.sch
 *
 * This needs more analysis.
 */
#if 1
			return related_same_repo(vcs_git);
#else
			return related_only_repo(vcs_git);
#endif
		else
			return related_other_repo(vcs_git);
	}

	return related_only_repo(vcs_git);
}


struct vcs_git *vcs_git_open(const char *revision, const char *name,
    const struct vcs_git *related)
{
	struct vcs_git *vcs_git = alloc_type(struct vcs_git);

	git_init_once();

	vcs_git->name = stralloc(name);
	vcs_git->revision = revision ? stralloc(revision) : NULL;
	vcs_git->related = related;

	if (try_related(vcs_git))
		return vcs_git;

	vcs_git->repo = select_repo(name);
	if (!vcs_git->repo) {
		error("%s: not found", name);
		goto fail;
	}
	progress(2, "using repository %s",
	    git_repository_path(vcs_git->repo));

	if (!revision)
		revision = "HEAD";
	vcs_git->commit = pick_revision(vcs_git->repo, revision);

	if (git_commit_tree(&vcs_git->tree, vcs_git->commit))
		pfatal_git(revision);

	if (!access_file_data(vcs_git, name))
		goto fail;

	return vcs_git;

fail:
	vcs_git_close(vcs_git);
	return 0;
}


/* ----- Hash table for avoiding redundant passes through ancestry --------- */


#define	HASH_BUCKETS	257


static struct hash_bucket {
	unsigned n;
	const void **refs;
} hash_buckets[HASH_BUCKETS];


static void hash_init(void)
{
	memset(hash_buckets, 0, sizeof(hash_buckets));
}


static bool hash_test(const void *ref)
{
	unsigned h = (unsigned long) ref % HASH_BUCKETS;
	struct hash_bucket *b = hash_buckets + h;
	unsigned i;

	for (i = 0; i != b->n; i++)
		if (b->refs[i] == ref)
			return 1;
	b->refs = realloc_type_n(b->refs, const void *, b->n + 1);
	b->refs[b->n] = ref;
	b->n++;
	return 0;
}


static void hash_free(void)
{
	unsigned i;

	for (i = 0; i != HASH_BUCKETS; i++)
		free((void *) hash_buckets[i].refs);
}


/* ----- Commit time ------------------------------------------------------- */


static int search(const char *root, const git_tree_entry *entry, void *payload)
{
	const git_oid *oid = payload;

	return git_oid_equal(git_tree_entry_id(entry), oid) ? -1 : 0;
}


/*
 * We have to use git_tree_walk instead of git_tree_entry_byid since the latter
 * doesn't recurse into sub-trees.
 */

static void recurse_time(const git_commit *commit, const git_oid *oid,
    time_t *best)
{
	unsigned n, i;
	git_commit *parent;
	git_tree *tree;
	const git_signature *sig;
	int res;

	sig = git_commit_author(commit);
#if 0
	if (!*best || sig->when.time < *best) {
		if (git_commit_tree(&tree, commit))
			pfatal_git("recurse_time");
		res = git_tree_walk(tree, GIT_TREEWALK_PRE, search,
		    (void *) oid);
		if (res)
			*best = sig->when.time;
	}
#else
	/*
	 * @@@ Stopping lookups when the object in question disappears from
	 * the tree may not be entirely correct (not sure whether it's
	 * actually wrong, though), but it can speed up things tremendously.
	 */
	if (git_commit_tree(&tree, commit))
		pfatal_git("recurse_time");
	res = git_tree_walk(tree, GIT_TREEWALK_PRE, search,
	    (void *) oid);
	if (!res)
		return;
	if (!*best || sig->when.time < *best)
		*best = sig->when.time;
#endif

	n = git_commit_parentcount(commit);
	for (i = 0; i != n; i++) {
		if (git_commit_parent(&parent, commit, i))
			pfatal_git("git_commit_parent");
		if (!hash_test(parent))
			recurse_time(parent, oid, best);
	}
}


#include "kicad/pl.h"


time_t vcs_git_time(void *ctx)
{
	const struct vcs_git *vcs_git = ctx;
	const git_oid *oid = git_object_id(vcs_git->obj);
	time_t t = 0;

	/*
	 * @@@ Retrieving time from git is rather slow, so we only do it if
	 * there is any chance we'll actually need it.
	 *
	 * What would be better solutions:
	 * - make the search more efficient (e.g., by avoiding searching the
	 *   ancestry of merges multiple times),
	 * - cache results,
	 * - lazy evaluation.
	 */
	if (date_override) {
		hash_init();
		recurse_time(vcs_git->commit, oid, &t);
		hash_free();
	}
	return t;
}


/* ----- Read -------------------------------------------------------------- */


bool vcs_git_read(void *ctx, struct file *file,
    bool (*parse)(const struct file *file, void *user, const char *line),
    void *user)
{
	const struct vcs_git *vcs_git = ctx;
	const char *end = vcs_git->data + vcs_git->size;
	const char *p = vcs_git->data;
	const char *nl;

	while (p != end) {
		nl = memchr(p, '\n', end - p);
		file->lineno++;
		if (!nl)
			return send_line(p, end - p, parse, user, file);
		if (!send_line(p, nl - p, parse, user, file))
			return 0;
		p = nl + 1;
	}
	return 1;
}


/* ----- Close ------------------------------------------------------------- */


void vcs_git_close(void *ctx)
{
	struct vcs_git *vcs_git = ctx;

	free((char *) vcs_git->name);
	free((char *) vcs_git->revision);
	free(vcs_git);
}
