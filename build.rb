#!/usr/bin/env ruby
# frozen_string_literal: true

require 'optparse'
require 'etc'
require 'open3'

BUILD_DIR = __dir__ + '/build'

EXE = "#{BUILD_DIR}/Exp"
EXE_TEST = "#{BUILD_DIR}/ExpTest"

BUILD = :build
RUN = :run
TEST = :test
COVR = :covr
INSTALL = :install

options = {
  BUILD => false,
  RUN => false,
  TEST => false,
  COVR => false,
  INSTALL => false
}

OptionParser.new do |opts|
  opts.on('-a', '--all', 'alias for build, test, & coverage') do |_v|
    options[BUILD] = true
    options[TEST] = true
    options[COVR] = true
  end

  opts.on('-b', '--build', 'invoke make and build') do |_v|
    options[BUILD] = true
  end

  opts.on('-r', '--run', 'invoke the executable') do |_v|
    options[RUN] = true
  end

  opts.on('-t', '--test', 'run tests') do |_v|
    options[TEST] = true
  end

  opts.on('-c', '--coverage', 'generate coverage after tests') do |_v|
    options[COVR] = true
  end

  opts.on('-i', '--install', 'install and/or build libraries') do |_v|
    options[INSTALL] = true
  end
end.parse!

def run(cmd)
  r, io = IO.pipe
  fork do
    system(cmd, out: io, err: :out)
  end
  io.close
  r.each_line { |l| puts l }
end

if options[BUILD]
  cwd = Dir.pwd
  Dir.chdir(BUILD_DIR)
  run "make -j#{Etc.nprocessors - 1}"
  Dir.chdir(cwd)
end

run EXE.to_s if options[RUN]

run EXE_TEST.to_s if options[TEST]

if options[COVR]
  cwd = Dir.pwd
  Dir.chdir(BUILD_DIR)
  run 'gcovr -r ../src --html --html-details -o coverage.html'
  Dir.chdir(cwd)
end

if options[INSTALL]
  run %(
    git submodule update -i
    sudo apt install liblua5.3-dev liblua5.3-0
  )
end
