#!/bin/bash

sourcePath="../tests/tutorial/"
docPath="./tutorial/"
command="python gentutorial.py"
fileList=(
	tutorial_variant
	tutorial_metatype
	tutorial_callable
	tutorial_metaclass
	tutorial_metaclass_inheritance
	tutorial_metarepo
)

for file in ${fileList[@]}
do
	${command} ${sourcePath}${file}.cpp ${docPath}${file}.md
done
