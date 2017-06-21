#
# Obtain external projects.
#

gem 'Mxx_ru', '>= 1.6.8'
require 'mxx_ru/externals'

MxxRu::git_externals :cpprestsdk do |e|
  e.url 'https://github.com/Microsoft/cpprestsdk.git'
  e.commit '0a9ca2c33236ccd6137a0645e152c97cf9a6384f'

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
  e.commit '4c15db48488cf292af76a8f4509686306b76449f'

  e.map_dir 'include' => 'dev/beast'
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


