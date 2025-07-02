import abs from '../src/index.ts'
import process from 'node:process'

const server = new abs.Server()

server.listen('\0Socket_cli', () => {
  console.debug('server is listening')
})

server.on('connection', (socket) => {
  console.debug('server on connection')

  socket.on('data', (data) => {
    console.debug('server on data', data.toString('utf-8'))
    socket.write('PONG @' + new Date().toISOString())
  })

  socket.on('close', () => {
    console.debug('server on close')
  })
})

process.on('exit', () => {
  server.close()
})
