{
  'targets': [
    {
      'target_name': 'abstract-socket',
      'sources': [ 'src/abstract-socket.cc' ],
      'include_dirs': [
        '<!(node -e "require(\'nan\')")'
      ],
      'conditions': [
        ['OS=="linux"', {
          'defines': [ '_GNU_SOURCE=1' ]
        }]
      ]
    }
  ]
}
