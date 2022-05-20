#!/usr/bin/perl

# metapp library

# Copyright (C) 2022 Wang Qi (wqking)

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#   http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

use strict;
use warnings;

my $args = {
	minHeadingCount => 6,
	minLevel => 2,
	maxLevel => 3,
	atFront => 0,
	clear => 0,
	beforeToc => '',
	afterToc => '',
};

&doMain;

sub doMain
{
	$args = &doParseArgs;
	
	foreach my $pattern (@{$args->{patterns}}) {
		&doProcessPattern($pattern);
	}
}

sub doParseArgs
{
	if(scalar(@ARGV) == 0) {
		&usage;
	}

	foreach my $arg (@ARGV) {
		if($arg =~ /^\-\-min\-headings=(.*)$/) {
			$args->{minHeadingCount} = $1 + 0;
		}
		elsif($arg =~ /^\-\-min\-level=(.*)$/) {
			$args->{minLevel} = $1 + 0;
		}
		elsif($arg =~ /^\-\-max\-level=(.*)$/) {
			$args->{maxLevel} = $1 + 0;
		}
		elsif($arg =~ /^\-\-front=(.*)$/) {
			$args->{atFront} = $1 + 0;
		}
		elsif($arg =~ /^\-\-clear$/) {
			$args->{clear} = 1;
		}
		elsif($arg =~ /^\-\-before\-toc=(.*)$/) {
			$args->{beforeToc} = &doNormalizeText($1);
			$args->{beforeToc} .= "\n" unless $args->{beforeToc} eq '';
		}
		elsif($arg =~ /^\-\-after\-toc=(.*)$/) {
			$args->{afterToc} = &doNormalizeText($1);
			$args->{afterToc} = "\n" . $args->{afterToc} unless $args->{afterToc} eq '';
		}
		elsif($arg =~ /^\-(.*)/) {
			die sprintf("Unknow option -%s.\n", $1);
		}
		else {
			push @{$args->{patterns}}, $arg;
		}
	}
	
	if(scalar(@{$args->{patterns}}) == 0) {
		die "Need input patterns.\n";
	}
	
	return $args;
}

sub doNormalizeText
{
	my ($text) = @_;
	
	$text =~ s/(?<!\\)\\n/\n/g;
	$text =~ s/\\\\/\\/g;
	
	return $text;
}

sub usage
{
	my ($message) = @_;
	
	print $message, "\n" if defined($message);
	print <<EOM;
addtoc2md version 0.1.
Github: https://github.com/wqking/markdownutil

addtoc2md adds table of content to Github flavored Markdown files.

usage: perl addtoc2md.pl [options] inputFile [more inputFile]
options:
    --min-headings=N      Set minimum heading count to N. If the valid heading
                          count is smaller than N, no TOC is created.
                          Default is 6.
    --min-level=N         Set minimum level to N. A heading level smaller than
                          N is invalid and not included in TOC. Default is 2.
    --max-level=N         Set maximum level to N. A heading level larger than N
                          is invalid and not included in TOC. Default is 3.
	--front=0             0 to put the TOC after the first valid heading. 1 to
                          put the TOC in front of the document. Default is 0.
                          The is option is not used if there is <!--toc--> mark
                          in the document.
	--clear               Remove all generated TOC. Don't generate new TOC.
	                       '<!--toc-->' is placed where the TOC was.
    --before-toc=S         Put the text S before the TOC. A new line is added
	                      between S and TOC. \n is replaced with new line break.
						  \\ is replaced with \.
    --after-toc=S          Put the text S after the TOC. A new line is added
	                      between TOC and S. \n is replaced with new line break.
						  \\ is replaced with \.
inputFile: The file name of the markdown file (.md). It can contain wildcard.
The inputFile can be specified multiple times.

Heading level: h1 has level 1, h2 has level 2, etc.
If there is <!--toc--> in the document (in a whole line), the TOC will be
put in place of <!--toc--> mark.
The option --front only takes effect when there is no <!--toc--> mark.
If there is <!--notoc--> in the document (in a whole line), the TOC will not
be generated.
EOM

	die "\n";
}

sub doProcessPattern
{
	my ($pattern) = @_;
	my @files = glob($pattern);
	foreach my $file (@files) {
		$file =~ s/\"//;
		next unless -f ($file);
		&doProcessFile($file, $args->{output});
	}
}

sub doProcessFile
{
	my ($fileName) = @_;
	
	return unless(open FH, '<' . $fileName);
	my @lineList = <FH>;
	close FH;
	
	my $lines = \@lineList;
	
	my $canAdd = (doGetHeadingCount($lines) >= $args->{minHeadingCount}) && ! doHasNoToc($lines) && ! $args->{clear};
	
	$lines = doRemoveToc($lines);
	$lines = doAddAnchors($lines, $canAdd) if($canAdd);
	
	if($canAdd) {
		my $toc = doBuildToc($lines);
		my $newLines = doAddTocAtTag($lines, $toc);
		if(defined($newLines)) {
			$lines = $newLines;
		}
		elsif($args->{atFront}) {
			unshift(@{$lines}, $toc);
		}
		else {
			$lines = doAddTocAfterFirstHeading($lines, $toc);
		}
	}
	
	if(open OFH, '>' . $fileName) {
		print OFH join('', @{$lines});
		close OFH;
	}
}

sub doRemoveFromLines
{
	my ($lines, $replacement) = @_;
	my $text = join('', @{$lines});
	$text =~ s/$replacement//;
	my @newLines = split(/\n/, $text);
	my @temp = map { $_ . "\n" } @newLines;
	return \@temp;
}

sub isValidLevel
{
	my ($level) = @_;
	
	return $level >= $args->{minLevel} && $level <= $args->{maxLevel};
}

sub doGetAnchorID
{
	my ($level, $count) = @_;
	
	return sprintf('a%d_%d', $level, $count);
}

sub doTrim
{
	my ($s) = @_;
	
	$s =~ s/^\s+//;
	$s =~ s/\s+$//;
	
	return $s;
}

sub doGetHeadingCount
{
	my ($inputLines) = @_;
	my $count = 0;
	doIterateHeadings($inputLines, sub {
		my ($userData) = @_;
		if(isValidLevel($userData->{level})) {
			++$count;
		}
		return $userData->{line};
	});
	return $count;
}

sub doHasNoToc
{
	my ($inputLines, $toc) = @_;
	
	my $has = 0;

	my $newLines = doIterateLines($inputLines, sub {
		my ($line) = @_;
		
		if($line =~ /^\s*\<\!\-\-\s*notoc\s*\-\-\>/) {
			$has = 1;
		}
		return $line;
	});
	
	return $has;
}

sub doAddTocAtTag
{
	my ($inputLines, $toc) = @_;
	
	my $added = 0;

	my $newLines = doIterateLines($inputLines, sub {
		my ($line) = @_;
		
		if($line =~ /^\s*\<\!\-\-\s*toc\s*\-\-\>/) {
			$added = 1;
			return ($toc);
		}
		return $line;
	});
	
	return $added ? $newLines : undef;
}

sub doAddTocAfterFirstHeading
{
	my ($inputLines, $toc) = @_;
	return doIterateHeadings($inputLines, sub {
		my ($userData) = @_;
		if($userData->{totalCount} == 1) {
			return ($userData->{line}, $toc);
		}
		return $userData->{line};
	});
}

sub doRemoveToc
{
	my ($inputLines) = @_;

	my $inToc = 0;
	
	return doIterateLines($inputLines, sub {
		my ($line) = @_;
		
		if($line =~ /^\s*\<\!\-\-\s*(begintoc|endtoc)\s*\-\-\>/) {
			$inToc = ($1 eq 'begintoc');
			if(! $inToc) {
				return "<!--toc-->\n";
			}
			return ();
		}
		return () if($inToc);
		return $line;
	});
}

sub doBuildToc
{
	my ($lines) = @_;
	
	my $data = {
		previousLevel => 10000,
		indentLevel => 0
	};
	
	my $result = '';
	
	$result .= "<!--begintoc-->\n";
	$result .= $args->{beforeToc};
	
	doIterateHeadings($lines, sub {
		my ($userData) = @_;
		my $level = $userData->{level};
		if(isValidLevel($level)) {
			if($level < $data->{previousLevel}) {
				--$data->{indentLevel};
				$data->{indentLevel} = 0 if $data->{indentLevel} < 0;
			}
			if($level > $data->{previousLevel}) {
				++$data->{indentLevel};
			}
			my $anchorId = &doGetAnchorID($userData->{level}, $userData->{counts}->[$userData->{level}]);
			$result .= '  ' x $data->{indentLevel};
			$result .= sprintf('* [%s](#%s)', doTrim($userData->{title}), $anchorId);
			$result .= "\n";
			
			$data->{previousLevel} = $level;
		}
		return $userData->{line};
	});

	$result .= $args->{afterToc};
	$result .= "<!--endtoc-->\n";
	
	return $result;
}

sub doAddAnchors
{
	my ($lines, $canAdd) = @_;
	
	return doIterateHeadings($lines, sub {
		my ($userData) = @_;
		my $resultLines = $userData->{resultLines};
		my $resultLineCount = scalar(@{$resultLines});
		if($resultLineCount > 0 && $resultLines->[$resultLineCount - 1] =~ /^\s*\<a\s+/) {
			pop(@{$resultLines});
		}
		if($canAdd && isValidLevel($userData->{level})) {
			my $anchorId = &doGetAnchorID($userData->{level}, $userData->{counts}->[$userData->{level}]);
			my $anchor = sprintf('<a id="%s"></a>' . "\n", $anchorId);
			return (
				$anchor,
				$userData->{line}
			);
		}
		return $userData->{line};
	});
}

sub doIterateLines
{
	my ($lines, $callback, $userData) = @_;

	my $resultLines = [];
	
	my $inCode = 0;
	
	foreach my $line (@{$lines}) {
		if($line =~ /^\s*```/) {
			$inCode = ! $inCode;
		}

		if($inCode) {
			push @{$resultLines}, $line;
		}
		else {
			push @{$resultLines}, $callback->($line, $userData, $resultLines);
		}
	}
	
	return $resultLines;
}

sub doIterateHeadings
{
	my ($inputLines, $callback) = @_;

	my $data = {
		level => 0,
		title => '',
		line => '',
		resultLines => undef,
		counts => [ 0, 0, 0, 0, 0, 0, 0, 0 ],
		totalCount => 0
	};
	
	return doIterateLines($inputLines, sub {
		my ($line, $userData, $resultLines) = @_;
		
		if($line =~ /^\s*(#+)(.*)/) {
			$data->{level} = length($1);
			$data->{title} = $2;
			$data->{line} = $line;
			$data->{resultLines} = $resultLines;
			
			++$data->{counts}->[$data->{level}];
			++$data->{totalCount};

			return $callback->($data);
		}
		return $line;
	});
}
