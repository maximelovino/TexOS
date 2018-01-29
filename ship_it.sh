#!/usr/bin/env bash
groupFolder=groupe3

mkdir -p $groupFolder

cp -r common/ $groupFolder/common/
cp -r kernel/ $groupFolder/kernel/
cp -r grub/ $groupFolder/grub/
cp -r tools/ $groupFolder/tools/
cp -r user/ $groupFolder/user/
cp makefile $groupFolder
cp questions_TP4.pdf $groupFolder

tar -cvzf $groupFolder.tar.gz $groupFolder

rm -rf $groupFolder
