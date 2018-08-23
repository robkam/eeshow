#!/bin/sh
#cat README 
#pushd ../..
cd=`pwd`
cd ../..

mkdir qi
cd qi/
clone git://projects.qi-hardware.com/kicad-libs.git
#popd
cd $cd
exit



jr@saturn:~/Dokumente/Neo900/kicad-et-al/kicad-git/ee> cat README 
External dependencies
---------------------

We use items from the Qi-Hardware libraries. If "ee" is in

/home/foo/ee

Then you should

cd /home
mkdir qi
cd qi
git clone git://projects.qi-hardware.com/kicad-libs.git

Thus, we get the following hierarchy:

/home
  foo
    ee
      hw
  qi
    kicad-libs
      components
      modules
jr@saturn:~/Dokumente/Neo900/kicad-et-al/kicad-git/ee> pushd ../..
~/Dokumente/Neo900/kicad-et-al ~/Dokumente/Neo900/kicad-et-al/kicad-git/ee
jr@saturn:~/Dokumente/Neo900/kicad-et-al> mkdir qi
jr@saturn:~/Dokumente/Neo900/kicad-et-al> cd qi/
jr@saturn:~/Dokumente/Neo900/kicad-et-al/qi> git clone git://projects.qi-hardware.com/kicad-libs.git
Klone nach 'kicad-libs'...
remote: Counting objects: 2252, done.
remote: Compressing objects: 100% (2239/2239), done.
remote: Total 2252 (delta 1479), reused 0 (delta 0)
Empfange Objekte: 100% (2252/2252), 324.72 KiB | 342.00 KiB/s, done.
Löse Unterschiede auf: 100% (1479/1479), done.
Prüfe Konnektivität... Fertig
jr@saturn:~/Dokumente/Neo900/kicad-et-al/qi> popd
