#!/usr/bin/env ruby
# frozen_string_literal: true

require 'filesystem'
require 'json'
require 'set'
require 'date'

SRC_DIR = "#{__dir__}/shaders/src"
OUT_DIR = "#{__dir__}/shaders/out"
SHADER_DIR = "#{__dir__}/json/shaders"

IMPORT_REGEX = /^#\s*import\s*"[\-\w.]+"$/.freeze

SUPPORTED_SHADERS = %i[vertex fragment].to_set.freeze

def forge_shader(shader, imported_files, successful_imports)
  src = nil

  imported_files.push(shader)

  File.open(shader) do |f|
    src = f.read
  end

  lines = []

  src.each_line do |line|
    if line.match?(IMPORT_REGEX)
      start = line.index('"') + 1
      stop = line.index('"', start)
      import_fn = "#{File.dirname(shader)}/#{line[start...stop]}"

      if imported_files.include?(import_fn)
        puts "circular dependency detected processing:\n'#{shader}'\nalready imported file:\n'#{import_fn}'"
        return false
      end

      next if successful_imports.include?(import_fn)

      return false unless forge_shader(import_fn, imported_files, successful_imports) do |code|
        lines.push(code)
      end

      successful_imports << import_fn
    else
      lines.push(line)
    end
  end

  yield lines.join if block_given?

  true
ensure
  imported_files.pop
end

shaders = Set.new

FileSystem.traverse(SHADER_DIR, '.json') do |fn|
  cfg = nil
  File.open(fn) do |f|
    data = f.read
    cfg = JSON.parse(data, symbolize_names: true)
  end

  unless cfg&.nil?
    cfg.each do |entry|
      files = entry[1]
      files.each do |kvp|
        type = kvp[0]
        next unless SUPPORTED_SHADERS.include?(type)

        file = kvp[1]
        shaders << file
      end
    end
  end
end

shaders.each do |shader|
  forge_shader("#{SRC_DIR}/#{shader}", [], Set.new) do |code|
    File.open("#{OUT_DIR}/#{shader}", 'w') do |f|
      f.write("/** Auto-Generated on #{Time.now} **/\n")
      f.write(code)
    end
  end
end
