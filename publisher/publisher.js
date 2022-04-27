const amqp = require('amqplib');
connect();
async function connect() {
try{
    const connection = await amqp.connect("amqp://localhost:5672");
    const channel = await connection.createChannel();
    const result =  await channel.assertQueue("jobs");
    channel.sendToQueue("jobs", Buffer.from("Hello World"));
    console.log(" [x] Sent 'Hello World'");
}
catch(ex){
   console.log(ex);
}
}