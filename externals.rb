#
# Obtain external projects.
#

gem 'Mxx_ru', '>= 1.6.8'
require 'mxx_ru/externals'

MxxRu::git_externals :cpprestsdk do |e|
  e.url 'https://github.com/Microsoft/cpprestsdk.git'
  e.commit 'e1933685bc2d5bb8395c6e6c7fd6850398654098'

  e.map_dir 'Release/include' => 'dev/cpprestsdk'
  e.map_dir 'Release/src' => 'dev/cpprestsdk'
  e.map_dir 'Release/cmake' => 'dev/cpprestsdk'
end

MxxRu::git_externals :restbed do |e|
  e.url 'https://github.com/Corvusoft/restbed.git'
  e.recursive
  e.tag '4.5'

  e.map_dir 'source' => 'dev/restbed'
  e.map_dir 'dependency' => 'dev/restbed'
  e.map_dir 'cmake' => 'dev/restbed'
end

MxxRu::git_externals :beast do |e|
  e.url 'https://github.com/vinniefalco/Beast.git'
  e.commit 'f68dc343e7c077caee8a95b2a59a2ccb9f979567'

  e.map_dir 'include' => 'dev/beast'
end

MxxRu::git_externals :pistache do |e|
  e.url 'https://github.com/oktal/pistache.git'
  e.commit '7b2b963d5e76a12afa2d921c5d12e5c980e054a0'

  e.map_dir 'include' => 'dev/pistache'
  e.map_dir 'src' => 'dev/pistache'
end

MxxRu::git_externals :web_cardan do |e|
  e.url 'https://github.com/masterspline/web_cardan.git'
  e.recursive
  # e.commit '58aed0a7c27a00b4125e770388a6407466551dc2'

  e.map_dir 'ext' => 'dev/web_cardan'
end

MxxRu::arch_externals :args_pistache_dep do |e|
  e.url 'https://github.com/Taywee/args/archive/6.0.4.tar.gz'

  e.map_file 'args.hxx' => 'dev/pistache/args/*'
end

MxxRu::arch_externals :asio_pure_asio_dep do |e|
  e.url 'https://github.com/chriskohlhoff/asio/archive/asio-1-11-0.tar.gz'
  e.sha1 '1be2489015a1e1c7b8666a5a803d984cdec4a12b'

  e.map_dir 'asio/include' => 'dev/pure_asio/asio'
end

MxxRu::arch_externals :args_pure_asio_dep do |e|
  e.url 'https://github.com/Taywee/args/archive/6.0.4.tar.gz'

  e.map_file 'args.hxx' => 'dev/pure_asio/args/*'
end

MxxRu::arch_externals :restinio do |e|
  e.url 'https://bitbucket.org/sobjectizerteam/restinio-0.2/downloads/restinio-0.2.1-full.tar.bz2'

  e.map_dir 'dev/restinio' => 'dev/restinio'
  e.map_file 'dev/nodejs/http_parser/CMakeLists.txt' => 'dev/restinio/nodejs/http_parser'

  # RESTinio dependencies:
  e.map_dir 'dev/args' => 'dev/restinio'

  e.map_dir 'dev/rapidjson' => 'dev/restinio'
  e.map_dir 'dev/rapidjson_mxxru' => 'dev/restinio'

  e.map_dir 'dev/json_dto' => 'dev/restinio'

  e.map_dir 'dev/fmt' => 'dev/restinio'
  e.map_dir 'dev/fmt_mxxru' => 'dev/restinio'

  e.map_dir 'dev/nodejs' => 'dev/restinio'

  e.map_dir 'dev/asio' => 'dev/restinio'
  e.map_dir 'dev/asio_mxxru' => 'dev/restinio'
end


