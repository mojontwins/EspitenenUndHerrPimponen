@echo off
if [%1]==[justcompile] goto :justcompile

del work\*.h
..\utils\mkts.exe mode=pals pals=..\gfx\palss0.png out=work\palss0.h label=palss0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts0.png out=work\palts0.h label=palts0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts1.png out=work\palts1.h label=palts1 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts2.png out=work\palts2.h label=palts2 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts3.png out=work\palts3.h label=palts3 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts4.png out=work\palts4.h label=palts4 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts5.png out=work\palts5.h label=palts5 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts6.png out=work\palts6.h label=palts6 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts7.png out=work\palts7.h label=palts7 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts8.png out=work\palts8.h label=palts8 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palts9.png out=work\palts9.h label=palts9 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palcuts0.png out=work\palcuts0.h label=palcuts0 silent
..\utils\mkts.exe mode=pals pals=..\gfx\palcuts1.png out=work\palcuts1.h label=palcuts1 silent
..\utils\mkts.exe mode=pals pals=..\gfx\paltitle.png out=work\paltitle.h label=paltitle silent
copy /b work\*.h assets\palettes.h

cd ..\gfx
..\utils\mkts.exe mode=scripted in=cut_patterns0.spt out=..\dev\tileset0.chr silent
..\utils\mkts.exe mode=scripted in=cut_patterns1.spt out=..\dev\tileset1.chr silent
..\utils\mkts.exe mode=scripted in=cut_patterns2.spt out=..\dev\tileset2.chr silent
..\utils\mkts.exe mode=scripted in=cut_patterns3.spt out=..\dev\tileset3.chr silent

..\utils\namgen.exe in=cuts0.png out=..\dev\assets\cuts0_rle.h pals=palcuts0.png chr=..\dev\tileset3.chr rle
..\utils\namgen.exe in=cuts1.png out=..\dev\assets\cuts1_rle.h pals=palcuts1.png chr=..\dev\tileset3.chr rle
..\utils\namgen.exe in=title.png out=..\dev\assets\title_rle.h pals=paltitle.png chr=..\dev\tileset3.chr rle

cd ..\map
..\utils\pemapcnv4.exe in=level0.map out=..\dev\assets\map0.h prefix=0 height=128 fixmappy
..\utils\pemapcnv4.exe in=level1.map out=..\dev\assets\map1.h prefix=1 height=128 fixmappy
..\utils\pemapcnv4.exe in=level2.map out=..\dev\assets\map2.h prefix=2 height=128 fixmappy
..\utils\pemapcnv4.exe in=level3.map out=..\dev\assets\map3.h prefix=3 height=128 fixmappy
..\utils\pemapcnv4.exe in=level4.map out=..\dev\assets\map4.h prefix=4 height=128 fixmappy
..\utils\pemapcnv4.exe in=level5.map out=..\dev\assets\map5.h prefix=5 height=128 fixmappy
..\utils\pemapcnv4.exe in=level6.map out=..\dev\assets\map6.h prefix=6 height=128 fixmappy
..\utils\pemapcnv4.exe in=level8.map out=..\dev\assets\map8.h prefix=8 height=128 fixmappy
..\utils\pemapcnv4.exe in=level9.map out=..\dev\assets\map9.h prefix=9 height=128 fixmappy

cd ..\enems
..\utils\eneexpp.exe level0.ene ..\dev\assets\enems0.h 0
..\utils\eneexpp.exe level1.ene ..\dev\assets\enems1.h 1
..\utils\eneexpp.exe level2.ene ..\dev\assets\enems2.h 2
..\utils\eneexpp.exe level3.ene ..\dev\assets\enems3.h 3
..\utils\eneexpp.exe level4.ene ..\dev\assets\enems4.h 4
..\utils\eneexpp.exe level5.ene ..\dev\assets\enems5.h 5
..\utils\eneexpp.exe level6.ene ..\dev\assets\enems6.h 6
..\utils\eneexpp.exe level8.ene ..\dev\assets\enems8.h 8
..\utils\eneexpp.exe level9.ene ..\dev\assets\enems9.h 9

:justcompile
cd ..\dev

del game.s

ca65 crt0-pantanow-CNROM.s -o crt0.o

cc65 -Oi game.c --add-source
ca65 game.s
ld65 -v -C nes-CNROM.cfg -o espinete.nes crt0.o game.o runtime.lib -Ln labels.txt

cc65 -Oi game.c --add-source -D LANG_ES
ca65 game.s
ld65 -v -C nes-CNROM.cfg -o espinete_sp.nes crt0.o game.o runtime.lib -Ln labels.txt

rem del game.s
del *.o

