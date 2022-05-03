const amqp = require('amqplib');

connect();
let vehicle = new Map();
async function connect() {
try{
    const connection = await amqp.connect("amqp://localhost:5672");
    const channel = await connection.createChannel();
    const result =  await channel.assertQueue("jobs"); 
    channel.consume("jobs", message => {
        let msg = message.content.toString().split(' ');
        if(!vehicle.has(msg[0])){
            vehicle.set(msg[0],msg[1]);
            channel.ack(message);
        }
        else{
            let t1= vehicle.get(msg[0]);
            let t2= msg[1];
            time = t2-t1;
            console.log(time);
            console.log(vehicle);
            vehicle.delete(msg[0]);
            channel.ack(message);
        }
    });
    console.log("Waiting for the messages.....");
}
catch(ex){
   console.log(ex);
}
}