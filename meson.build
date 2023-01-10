project(
  'asio_server_client',
  'cpp',
  version : '0.0.1',
  default_options : [
    'buildtype=debugoptimized',
    'b_lto=true',
    'b_pie=true',
    'cpp_std=c++20',
    'warning_level=3',
    'werror=true',
  ],
)

asio_dep = dependency('asio', required: true)
fmt_dep = dependency('fmt', required: true)

cpp_args = [
  '-fstack-protector-strong',
  '-D_FORTIFY_SOURCE=2',
]

link_args = [
  '-Wl,-z,now',
]

executable(
  'asio_client',
  'client.cpp',
  cpp_args: cpp_args,
  dependencies: [
    asio_dep,
    fmt_dep,
  ],
  link_args: link_args,
)

executable(
  'asio_server',
  'server.cpp',
  cpp_args: cpp_args,
  dependencies: [
    asio_dep,
    fmt_dep,
  ],
  link_args: link_args,
)
