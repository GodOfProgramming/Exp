#!/bin/bash

build=0
slow_build=0
run=0
run_tests=0
gen_coverage=0
setup=0
clean=0

proj_root="$(dirname "$0")"
build_dir="${proj_root}/build"

while getopts 'hicbstgra' flag; do
	case "$flag" in
		h)
			echo 'build.sh [flags]'
			exit 0
			;;
    i)
      setup=1
      ;;
    c)
      clean=1
      ;;
		b)
			build=1
			;;
		s)
			slow_build=1
			;;
		t)
			run_tests=1
			;;
		g)
			gen_coverage=1
			;;
    r)
      run=1
      ;;
		a)
			build=1
			run_tests=1
			gen_coverage=1
			;;
		*)
			echo 'invalid argument given'
			exit 1
			;;
	esac
done

shift $((OPTIND-1))

if [ $clean -eq 1 ]; then
	cur_dir=$(pwd)
	cd "${build_dir}"
	make -j$(($(nproc) - 1)) clean || exit $?
	cd "${cur_dir}"
fi

if [ $build -eq 1 ]; then
	cur_dir=$(pwd)
	cd "${build_dir}"
	if [ $slow_build -eq 0 ]; then
		make -j$(($(nproc) - 1)) || exit $?
	else
		make || exit $?
	fi
	cd "${cur_dir}"
fi

if [ $run_tests -eq 1 ]; then
	cmd=${build_dir}/ExpTest
	if [ ! -z "$1" ]; then
		${cmd} --gtest_filter="$1" || exit $?
	else
		${cmd} || exit $?
	fi
fi

if [ $gen_coverage -eq 1 ]; then
	cur_dir=$(pwd)
	cd "${build_dir}"
	gcovr -r ../src --html --html-details -o coverage.html || exit $?
	cd "${cur_dir}"
fi

if [ $setup -eq 1 ]; then
  git submodule update --init --recursive
fi

if [ $run -eq 1 ]; then
	cmd=${build_dir}/Exp
  ${cmd} $@ || exit $?
fi
