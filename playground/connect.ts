import abs from '../src/index.ts'

const socket = abs.createConnection('\0Socket_cli')

let timer: NodeJS.Timeout

socket.on('connect', () => {
  console.debug('client on connect')

  timer = setInterval(() => {
    socket.write('PING @' + new Date().toISOString())
  }, 2000)
})

socket.on('data', (data) => {
  console.debug('client on data', data.toString('utf-8'))
})

socket.on('close', () => {
  console.debug('client on close')
  if (timer) {
    clearInterval(timer)
  }
})
