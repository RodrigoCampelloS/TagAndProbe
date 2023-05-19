## \file
## \ingroup tutorial_pyroot_legacy
## \notebook
## This macro displays the ROOT Directory data structure
##
## \macro_image
## \macro_code
##
## \author Wim Lavrijsen

from ROOT import TCanvas, TPaveLabel, TPaveText, TText, TArrow, TLine
from ROOT import gROOT

c1 = TCanvas( 'c1', 'ROOT FilDir description', 700, 900 )
c1.Range( 1, 1, 19, 24.5 )

title = TPaveLabel( 4, 23, 16, 24.2, 'ROOT File/Directory/Key description' )
title.SetFillColor( 16 )
title.Draw()

keycolor = 42
dircolor = 21
objcolor = 46
file = TPaveText( 2, 19, 6, 22 )
file.SetFillColor( 39 )
file.Draw()
file.SetTextSize( 0.04 )
file.AddText( 'TFile' )
file.AddText( 'Header' )
arrow = TArrow( 6, 20.5, 17, 20.5, 0.02, '|>' )
arrow.SetFillStyle( 1001 )
arrow.SetLineWidth( 2 )
arrow.Draw()
free = TPaveText( 8, 20, 11, 21 )
free.SetFillColor( 18 )
free.Draw()
free.AddText( 'First:Last' )
free2 = TPaveText( 12, 20, 15, 21 )
free2.SetFillColor( 18 )
free2.Draw()
free2.AddText( 'First:Last' )
tfree = TText( 6.2, 21.2, 'fFree = TList of free blocks' )
tfree.SetTextSize( 0.02 )
tfree.Draw()
tkeys = TText( 5.2, 18.2, 'fKeys = TList of Keys' )
tkeys.SetTextSize( 0.02 )
tkeys.Draw()
tmemory = TText( 3.2, 15.2, 'fListHead = TList of Objects in memory' )
tmemory.SetTextSize( 0.02 )
tmemory.Draw()

arrow.DrawArrow( 5, 17, 17, 17, 0.02, '|>' )
line = TLine( 5, 19, 5, 17 )
line.SetLineWidth( 2 )
line.Draw()
key0 = TPaveText( 7, 16, 10, 18 )
key0.SetTextSize( 0.04 )
key0.SetFillColor( keycolor )
key0.AddText( 'Key 0' )
key0.Draw()
key1 = TPaveText( 12, 16, 15, 18 )
key1.SetTextSize( 0.04 )
key1.SetFillColor( keycolor )
key1.AddText( 'Key 1' )
key1.Draw()
line.DrawLine( 3, 19, 3, 14 )
line.DrawLine( 3, 14, 18, 14 )
obj0 = TPaveText( 5, 13, 8, 15 )
obj0.SetFillColor( objcolor )
obj0.AddText( 'Object' )
obj0.Draw()
dir1 = TPaveText( 10, 13, 13, 15 )
dir1.SetFillColor( dircolor )
dir1.AddText( 'SubDir' )
dir1.Draw()
obj1 = TPaveText( 15, 13, 18, 15 )
obj1.SetFillColor( objcolor )
obj1.AddText( 'Object' )
obj1.Draw()
arrow.DrawArrow( 12, 11, 17, 11, 0.015, '|>' )
arrow.DrawArrow( 11, 9, 17, 9, 0.015, '|>' )
line.DrawLine( 12, 13, 12, 11 )
line.DrawLine( 11, 13, 11, 9 )
key2 = TPaveText( 14, 10.5, 16, 11.5 )
key2.SetFillColor( keycolor )
key2.AddText( 'Key 0' )
key2.Draw()
obj2 = TPaveText( 14, 8.5, 16, 9.5 )
obj2.SetFillColor( objcolor )
obj2.AddText( 'Object' )
obj2.Draw()
ldot = TLine( 10, 15, 2, 11 )
ldot.SetLineStyle( 2 )
ldot.Draw()
ldot.DrawLine( 13, 15, 8, 11 )
ldot.DrawLine( 13, 13, 8, 5 )
dirdata = TPaveText( 2, 5, 8, 11 )
dirdata.SetTextAlign( 12 )
dirdata.SetFillColor( dircolor )
dirdata.Draw()
dirdata.SetTextSize( 0.015 )
dirdata.AddText( 'fModified: True if directory is modified' )
dirdata.AddText( 'fWritable: True if directory is writable' )
dirdata.AddText( 'fDatimeC: Creation Date/Time' )
dirdata.AddText( 'fDatimeM: Last mod Date/Time' )
dirdata.AddText( 'fNbytesKeys: Number of bytes of key' )
dirdata.AddText( 'fNbytesName : Header length up to title' )
dirdata.AddText( 'fSeekDir: Start of Directory on file' )
dirdata.AddText( 'fSeekParent: Start of Parent Directory' )
dirdata.AddText( 'fSeekKeys: Pointer to Keys record' )
keydata = TPaveText( 10, 2, 17, 7 )
keydata.SetTextAlign( 12 )
keydata.SetFillColor( keycolor )
keydata.Draw()
ldot.DrawLine( 14, 11.5, 10, 7 )
ldot.DrawLine( 16, 11.5, 17, 7 )
keydata.SetTextSize( 0.015 )
keydata.AddText( 'fNbytes: Size of compressed Object' )
keydata.AddText( 'fObjLen: Size of uncompressed Object' )
keydata.AddText( 'fDatime: Date/Time when written to store' )
keydata.AddText( 'fKeylen: Number of bytes for the key' )
keydata.AddText( 'fCycle : Cycle number' )
keydata.AddText( 'fSeekKey: Pointer to Object on file' )
keydata.AddText( 'fSeekPdir: Pointer to directory on file' )
keydata.AddText( 'fClassName: "TKey"' )
keydata.AddText( 'fName: Object name' )
keydata.AddText( 'fTitle: Object Title' )

c1.Update()
