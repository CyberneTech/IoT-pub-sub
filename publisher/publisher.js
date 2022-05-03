const amqp = require('amqplib');
connect();
const msg = '1fg582 234930';
async function connect() {
try{
    const connection = await amqp.connect("amqp://localhost:5672");
    const channel = await connection.createChannel();
    const result =  await channel.assertQueue("jobs");
    channel.sendToQueue("jobs", Buffer.from(msg));
    }
catch(ex){
   console.log(ex);
}
}