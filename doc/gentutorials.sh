#!/bin/bash

sourcePath="../tests/tutorial"
tutorialDestPath="./tutorial"
command="python gentutorial.py"
tutorialFileList=(
	tutorial_start
	tutorial_variant
	tutorial_metatype
	tutorial_callable
	tutorial_metaclass
	tutorial_metaclass_inheritance
	tutorial_metarepo
)

for file in ${tutorialFileList[@]}
do
	${command} ${sourcePath}/${file}.cpp ${tutorialDestPath}/${file}.md
done

metaTypeFileList=(
	accessor
	constructor
	default_args_function
	variadic_function
)

for file in ${metaTypeFileList[@]}
do
	${command} ${sourcePath}/doc/metatypes/doc_${file}.cpp ./metatypes/${file}.md
done

interfaceFileList=(
	metaaccessible
	metacallable
	metaclass
	metaenum
	metaindexable
	metaiterable
	metamappable
	metamember
	metastreaming
	metauser
)

for file in ${interfaceFileList[@]}
do
	${command} ${sourcePath}/doc/interfaces/doc_${file}.cpp ./interfaces/${file}.md
done

docFileList=(
	core_concepts
	meta_interface_overview
	variant
	metatype
	declaremetatype
	metarepo
	registered_items
)

for file in ${docFileList[@]}
do
	${command} ${sourcePath}/doc/doc_${file}.cpp ./${file}.md
done

