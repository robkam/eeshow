/*
 * file/git-hist.c - Retrieve revision history from GIT repo
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define _GNU_SOURCE
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>	/* for vcs_long_for_pango */
#include <alloca.h>
#include <assert.h>

#include <git2.h>

#include "misc/util.h"
#include "misc/diag.h"
#include "file/file.h"
#include "file/git-util.h"
#include "file/git-file.h"
#include "file/git-hist.h"


/*
 * @@@ we assume to have a single head. That isn't necessarily true, since
 * each open branch has its own head. Getting this right is for further study.
 */


struct branch {
	const char *name;
	git_commit *commit;
};

struct vcs_history {
	git_repository *repo;

	struct vcs_hist **heads;
	unsigned n_heads;

	struct branch *branches;
	unsigned n_branches;

	struct vcs_hist *history;	/* any order */

	struct vcs_hist **sorted_hist;
	unsigned n_hist;

	unsigned max_threads;
};


/* ----- History retrieval ------------------------------------------------- */


static struct vcs_hist *new_commit(struct vcs_history *history, unsigned branch)
{
	struct vcs_hist *h;

	h = alloc_type(struct vcs_hist);
	h->commit = NULL;
	h->branch = branch;
	h->branches = NULL;
	h->n_branches = 0;
	h->newer = NULL;
	h->n_newer = 0;
	h->older = NULL;
	h->n_older = 0;
	h->next = history->history;
	history->history = h;
	return h;
}


static struct branch *get_branches(git_repository *repo, unsigned *res_n)
{
	git_branch_iterator *iter;
	git_reference *ref;
	git_branch_t type;
	git_object *obj;
	struct branch *res = NULL;
	unsigned n = 0;

	if (git_branch_iterator_new(&iter, repo, GIT_BRANCH_ALL))
		pfatal_git("git_branch_iterator");
	while (!git_branch_next(&ref, &type, iter)) {
		/*
		 * @@@ is it okay to just ignore symbolic references ?
		 * E.g., remotes/origin/HEAD -> origin/master
		 */
		if (git_reference_type(ref) != GIT_REF_OID)
			continue;
		if (git_reference_peel(&obj, ref, GIT_OBJ_COMMIT))
			pfatal_git("git_reference_peel");

		res = realloc_type_n(res, struct branch, n + 1);
		if (git_branch_name(&res[n].name, ref))
			pfatal_git("git_branch_name");
		res[n].commit = (git_commit *) obj;
		n++;
	}
	git_branch_iterator_free(iter);
	*res_n = n;
	return res;
}


static const char **matching_branches(struct vcs_history *history,
    const struct git_commit *commit, unsigned *res_n)
{
	unsigned i, n = 0;
	const char **res;

	for (i = 0; i != history->n_branches; i++)
		if (history->branches[i].commit == commit)
			n++;
	res = alloc_type_n(const char *, n);
	n = 0;
	for (i = 0; i != history->n_branches; i++)
		if (history->branches[i].commit == commit)
			res[n++] = history->branches[i].name;
	*res_n = n;
	return res;
}


static void uplink(struct vcs_hist *down, struct vcs_hist *up)
{
	down->newer = realloc_type_n(down->newer, struct vcs_hist *,
	    down->n_newer + 1);
	down->newer[down->n_newer++] = up;
}


static struct vcs_hist *find_commit(struct vcs_history *history,
    const git_commit *commit)
{
	struct vcs_hist *h;

	/*
	 * @@@ should probably use
	 * git_oid_equal(git_commit_id(a), git_commit_id(b))
	 */

	for (h = history->history; h; h = h->next)
		if (h->commit == commit)
			break;
	return h;
}


static void recurse(struct vcs_history *history, struct vcs_hist *h,
    unsigned n_branches, unsigned depth)
{
	unsigned n, i;

	n = git_commit_parentcount(h->commit);
	if (verbose > 2)
		progress(3, "commit %p: %u + %u", h->commit, n_branches, n);

	n_branches--;

	h->older = alloc_type_n(struct vcs_hist *, n);
	h->n_older = n;

	for (i = 0; i != n; i++) {
		git_commit *commit;
		struct vcs_hist *found = NULL;

		if (git_commit_parent(&commit, h->commit, i))
			pfatal_git("git_commit_parent");
		found = find_commit(history, commit);
		if (found) {
			uplink(found, h);
			h->older[i] = found;
		} else {
			struct vcs_hist *new;

			new = new_commit(history, n_branches);
			new->commit = commit;
			new->branches = matching_branches(history, commit,
			    &new->n_branches);
			h->older[i] = new;
			n_branches++;
			uplink(new, h);
			if (depth != 1)
				recurse(history, new, n_branches,
				    depth ? depth - 1 : 0);
		}
	}
}


bool vcs_git_try(const char *path)
{
	git_repository *repo;
	struct file file;

	git_init_once();

	/*
	 * We could just call file_open_vcs and let it check if the repo even
	 * exists, but then it would complain about the file not existing,
	 * which would be at best confusing.
	 */
	if (git_repository_open_ext_caching(&repo, path,
	    GIT_REPOSITORY_OPEN_CROSS_FS, NULL)) {
		perror_git(path);
		return 0;
	}
	if (git_repository_is_empty(repo)) {
		error("%s: repository is empty", path);
		return 0;
	}

	/* check that the file is really in the repo */
	if (!file_open_vcs(&file, path))
		return 0;
	file_close(&file);
	return 1;
}


static void add_head(struct vcs_history *history, struct vcs_hist *head)
{
	history->heads = realloc_type_n(history->heads, struct vcs_hist *,
	    history->n_heads + 1);
	history->heads[history->n_heads] = head;
	history->n_heads++;
}


static void merge_head(struct vcs_history *history, git_commit *commit,
    unsigned depth)
{
	struct vcs_hist *new;

	if (find_commit(history, commit))
		return;
	new = new_commit(history, 0);
	new->commit = commit;
	new->branches = matching_branches(history, new->commit,
	    &new->n_branches);
	recurse(history, new, 1, depth);
	add_head(history, new);
}


struct vcs_history *vcs_git_history(const char *path, unsigned depth)
{
	struct vcs_history *history;
	struct vcs_hist *head, *dirty;
	git_oid oid;
	unsigned i;

	history = alloc_type(struct vcs_history);
	history->history = NULL;
	history->heads = NULL;
	history->n_heads = 0;
	history->sorted_hist = NULL;
	history->n_hist = 0;

	head = new_commit(history, 0);

	git_init_once();

	if (git_repository_open_ext_caching(&history->repo, path,
	    GIT_REPOSITORY_OPEN_CROSS_FS, NULL))
		pfatal_git(path);

	history->branches = get_branches(history->repo, &history->n_branches);

	if (git_reference_name_to_id(&oid, history->repo, "HEAD"))
		pfatal_git(git_repository_path(history->repo));

	if (git_commit_lookup(&head->commit, history->repo, &oid))
		pfatal_git(git_repository_path(history->repo));

	head->branches = matching_branches(history, head->commit,
	    &head->n_branches);
	recurse(history, head, 1, depth);

	if (git_repo_is_dirty(history->repo)) {
		dirty = new_commit(history, 0);
		dirty->older = alloc_type(struct vcs_hist *);
		dirty->older[0] = head;
		dirty->n_older = 1;
		uplink(head, dirty);
		add_head(history, dirty);
	} else {
		add_head(history, head);
	}

	for (i = 0; i != history->n_branches; i++)
		merge_head(history, history->branches[i].commit, depth);

	return history;
}


/* ----- See if history is empty ------------------------------------------- */


bool vcs_is_empty(const struct vcs_history *history)
{
	return !history->n_heads;
}


/* ----- Get information about commit -------------------------------------- */


char *vcs_git_get_rev(struct vcs_hist *h)
{
	const git_oid *oid = git_commit_id(h->commit);
	char *s = alloc_size(GIT_OID_HEXSZ + 1);

	return git_oid_tostr(s, GIT_OID_HEXSZ + 1, oid);
}


const char *vcs_git_summary(const struct vcs_hist *h)
{
	const char *summary;

	if (!h->commit)
		return "Uncommitted changes";
	summary = git_commit_summary(h->commit);
	if (summary)
		return summary;

	pfatal_git("git_commit_summary");
}


/* Pango supports alpha values in background color only since 1.38 */

#if PANGO_VERSION_MAJOR >= 1 || PANGO_VERSION_MINOR >= 38
#define	BRANCH_BG	"background=\"#00e00080\""
#else
#define	BRANCH_BG	"background=\"#00e000\""
#endif


char *vcs_git_summary_for_pango(const struct vcs_hist *h,
    char *(*formatter)(const char *fmt, ...))
{
	const char *summary;

	if (!h->commit)
		return formatter("<small>Uncommitted changes</small>");
	summary = git_commit_summary(h->commit);
	if (!summary)
		pfatal_git("git_commit_summary");

	if (h->n_branches)
		return formatter(
		    "<small><span " BRANCH_BG "><b>%s</b>%s</span> %s</small>",
		    h->branches[0], h->n_branches > 1 ? "+" : "", summary);
	else
		return formatter("<small>%s</small>", summary);
}


static char *append(char *s, char *add)
{
	unsigned len_s, len_add;

	if (!s)
		return add;
	len_s = strlen(s);
	len_add = strlen(add);
	s = realloc_size(s, len_s + len_add + 1);
	strcpy(s + len_s, add);
	free(add);
	return s;
}


/*
 * @@@ This one is a bit inconvenient. It depends both on the information the
 * VCS provides, some of which is fairly generic, but some may not be, and
 * the very specific constraints imposed by the markup format of Pango.
 */

char *vcs_git_long_for_pango(const struct vcs_hist *h,
    char *(*formatter)(const char *fmt, ...), bool full)
{
	git_buf buf = { 0 };
	const git_signature *sig;
	char *s = NULL;
	unsigned i;

	if (!h->commit)
		return stralloc("Uncommitted changes");
	if (git_object_short_id(&buf, (git_object *) h->commit))
		goto fail;
	sig = git_commit_author(h->commit);

	for (i = 0; i != h->n_branches; i++)
		s = append(s, formatter(
		    "%s<span " BRANCH_BG "><b> %s </b></span>",
		    i ? " " : "", h->branches[i]));
	s = append(s, formatter(
	    "%s<b>%s</b> %s%s &lt;%s&gt;<small>\n%s%s</small>",
	    h->n_branches ? "\n" : "",
	    buf.ptr, ctime(&sig->when.time), sig->name, sig->email,
	    full ? "\n" : "",
	    full ? git_commit_message(h->commit) :
	    git_commit_summary(h->commit)));
	git_buf_free(&buf);
	return s;

fail:
	pfatal_git("vcs_git_long_for_pango");
}


/* ----- Sorted commit history --------------------------------------------- */


static bool older(const struct vcs_hist *a, const struct vcs_hist *b)
{
	unsigned i;

	for (i = 0; i != a->n_newer; i++) {
		if (a->newer[i] == b)
			return 1;
		if (older(a->newer[i], b))
			return 1;
	}
	return 0;
}


static bool newer(const struct vcs_hist *a, const struct vcs_hist *b)
{
	unsigned i;

	for (i = 0; i != a->n_older; i++) {
		if (a->older[i] == b)
			return 1;
		if (newer(a->older[i], b))
			return 1;
	}
	return 0;
}


static int comp_hist(const void *a, const void *b)
{
	const struct vcs_hist *ha = *(const struct vcs_hist **) a;
	const struct vcs_hist *hb = *(const struct vcs_hist **) b;
	time_t ta, tb;

	if (!ha->commit)
		return -1;
	if (!hb->commit)
		return 1;

	ta = git_commit_time(ha->commit);
	tb = git_commit_time(hb->commit);

	assert(ha->commit != hb->commit);
	if (older(ha, hb))
		return 1;
	if (newer(ha, hb))
		return -1;
	return ta == tb ? 0 : ta < tb ? 1 : -1;
}


static void sort_history(struct vcs_history *history)
{
	struct vcs_hist **vec;
	struct vcs_hist *h;
	unsigned n = 0;

	for (h = history->history; h; h = h->next)
		n++;

	vec = alloc_type_n(struct vcs_hist *, n);
	n = 0;
	for (h = history->history; h; h = h->next)
		vec[n++] = h;

	qsort(vec, n, sizeof(const struct vcs_hist *), comp_hist);

	history->sorted_hist = vec;
	history->n_hist = n;
}


/* ----- Assign thread positions ------------------------------------------- */


static void grow(const struct vcs_hist ***t, unsigned *n)
{
	*t = realloc_type_n(*t, const struct vcs_hist *, *n + 1);
	(*t)[*n] = NULL;
	(*n)++;
}


static unsigned find_pos(const struct vcs_hist ***t, unsigned *n,
    const struct vcs_hist *h)
{
	unsigned pos, i;

	for (pos = 0; pos != *n; pos++)
		for (i = 0; i != h->n_newer; i++)
			if (h->newer[i] == (*t)[pos])
				return pos;
	for (pos = 0; pos != *n; pos++)
		if (!(*t)[pos])
			return pos;
	grow(t, n);
	return *n  - 1;
}


static void clear_children(const struct vcs_hist **t, unsigned n,
    const struct vcs_hist *h)
{
	unsigned i, j;

	for (i = 0; i != h->n_newer; i++) {
		for (j = 0; j != n; j++)
			if (t[j] == h->newer[i])
				goto clear;
		fatal("child thread not found");
clear:
		t[j] = NULL;
	}
}


static unsigned assign_threads(struct vcs_history *history)
{
	struct vcs_hist **h;
	unsigned n = 0;
	const struct vcs_hist **t = 0;
	unsigned i, j;

	for (h = history->sorted_hist;
	    h != history->sorted_hist + history->n_hist; h++) {
		(*h)->n_threads = n;
		(*h)->threads = alloc_type_n(const struct vcs_hist *, n);
		memcpy((*h)->threads, t, sizeof(const struct vcs_hist *) * n);

		(*h)->pos = find_pos(&t, &n, *h);
		clear_children(t, n, *h);
		for (i = 0; i != (*h)->n_older; i++) {
			if (!i) {
				t[(*h)->pos] = *h;
				continue;
			}
			for (j = 0; j != n; j++)
				if (!t[j]) {
					t[j] = *h;
					break;
				}
			if (j == n) {
				grow(&t, &n);
				t[n - 1] = *h;
			}
		}
	}
	return n;
}


/* ----- Thread vector ----------------------------------------------------- */


unsigned threads_number(const struct vcs_history *history)
{
	return history->max_threads;
}


enum thread *threads_classify(const struct vcs_history *history,
    const struct vcs_hist *h, const struct vcs_hist *next)
{
	enum thread *t;
	unsigned i, j;

	t = alloc_type_n(enum thread, history->max_threads);
	for (i = 0; i != history->max_threads; i++)
		t[i] = thread_none;
	for (i = 0; i != h->n_threads; i++)
		for (j = 0; j != h->n_newer; j++)
			if (h->newer[j] == h->threads[i] &&
			    (!next || h->newer[j] != next->threads[i]))
				t[i] |= thread_newer;
	if (next) {
		for (i = 0; i != next->n_threads; i++) {
			if (next->threads[i] == h)
				t[i] |= thread_older;
			else if (next->threads[i])
				t[i] |= thread_other;
		}
	} else {
		for (i = 0; i != h->n_threads; i++)
			if (h->threads[i]) {
				/* we don't have |-. type of branches */
				assert(!(t[i] & thread_older));
				t[i] |= thread_other;
			}
	}
	while (i != history->max_threads) {
		t[i] = thread_none;
		i++;
	}
	t[h->pos] |= thread_self;

	return t;
}


/* ----- Iteration --------------------------------------------------------- */


void hist_iterate(struct vcs_history *history,
    void (*fn)(void *user, struct vcs_hist *h, const struct vcs_hist *next),
    void *user)
{
	unsigned i;

	if (!history->sorted_hist) {
		sort_history(history);
		history->max_threads = assign_threads(history);
	}
	for (i = 0; i != history->n_hist; i++)
		fn(user, history->sorted_hist[i],
		    i + 1 == history->n_hist ?
		    NULL : history->sorted_hist[i + 1]);
}


/* ----- Textual dump (mainly for debugging) ------------------------------- */


/*
 * useful:
 * http://stackoverflow.com/questions/12132862/how-do-i-get-the-name-of-the-current-branch-in-libgit2
 */

static void dump_one(void *user, struct vcs_hist *h,
    const struct vcs_hist *next)
{
	struct vcs_history *history = user;
	git_buf buf = { 0 };
	enum thread *t;
	unsigned n = threads_number(history);
	unsigned i, j;
	bool before, here, after;

	t = threads_classify(history, h, next);
	for (i = 0; i != n; i++) {
		before = 0;
		for (j = 0; j != i; j++)
			if (t[j] &
			    (thread_self | thread_newer | thread_older)) {
				before = 1;
				break;
			}
		here = t[i] & (thread_self | thread_newer | thread_older);
		after = 0;
		for (j = i + 1; j != n; j++)
			if (t[j] &
			    (thread_self | thread_newer | thread_older)) {
				after = 1;
				break;
			}
		if (t[i] & thread_self)
			putchar('o');
		else if ((t[i] & (thread_newer | thread_older)) ==
		    (thread_newer | thread_older))
			putchar('+');
		else if (t[i] & thread_newer)
			putchar('\'');
		else if (t[i] & thread_older)
			putchar('.');
		else if (t[i] & thread_other)
			putchar('|');
		else
			putchar(after && before ? '-' : ' ');
		putchar(after && (here || before) ? '-' : ' ');
	}
	free(t);

	if (!h->commit) {
		printf("  dirty\n");
		return;
	}

	if (git_object_short_id(&buf, (git_object *) h->commit))
		pfatal_git("git_object_short_id");

	printf("  %s  ", buf.ptr);
	git_buf_free(&buf);


	if (h->n_branches) {
		printf("[");
		for (i = 0; i != h->n_branches; i++)
			printf("%s%s", i ? " " : "", h->branches[i]);
		printf("] ");
	}

	printf("%s\n", vcs_git_summary(h));
}


void dump_hist(struct vcs_history *history)
{
	hist_iterate(history, dump_one, history);
}
