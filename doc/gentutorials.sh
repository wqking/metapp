#!/bin/bash

sourcePath="../tests/tutorial/"
docPath="./tutorial/"
command="python gentutorial.py"

for file in tutorial_callable tutorial_metaclass
do
	${command} ${sourcePath}${file}.cpp ${docPath}${file}.md
done
