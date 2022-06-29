const amqp = require('amqplib');
const { Client } = require('@elastic/elasticsearch');
require('dotenv').config();


let vehicle = new Map();
const d = 15;              //fixed distance b/w 2 RFIDs

```
{
    vehicle_id: string,
    timestamp: integer
}
```

async function connect() {
    try{
        const connection = await amqp.connect("amqp://localhost:5672");
        const channel = await connection.createChannel();
        const result =  await channel.assertQueue("jobs"); 
    }
    catch(err){
        console.log("Couldn't connect to the message queue successfully");
    }

    try{
        channel.consume("jobs", message => {
            let msg = message.content.toString().split(' ');
            if(!vehicle.has(msg[0])){
                vehicle.set(msg[0],msg[1]);
                channel.ack(message);
            }
            else{
                let t1= vehicle.get(msg[0]);
                let t2= msg[1];
                //calculate the speed of vehicle(id)
                time = t2-t1;
                speed = d/time;

                //removing the id from map and clearing the queue
                vehicle.delete(msg[0]);
                channel.ack(message);

                //returning id and speed 
                return [msg[0],time];
            }
        });
    }
    catch(ex){
        console.log('Could not compute speed');
    }
}
connect().then(
    function(vehicle){
        console.log(vehicle);
    }
)

try{
    const client = new Client({
        cloud: {
            id: process.env.CLOUD_ID
        },
        auth: {
            username: process.env.USER_NAME,
            password: process.env.PASSWORD
        }
    })
    async function run () {
      
      await client.indices.putMapping({
        index: 'traffic-data',
        body: {
          properties: {
            location: {
              type: 'geo_point'
            },
             timestamp: {
              type: 'date',
              format: "yyyy-MM-dd HH:mm:ss||epoch_millis"
            }
          }
        }
      })
    
      var epoch_millis = moment().valueOf();
      await client.index({
        index: 'traffic-data',
        document: {
          vehicle_id: arr[0],
          speed: arr[1],
          location: [77.5946,12.9716],      //fixed coordinates sensors at given location
          timestamp: epoch_millis
        }
      })
    }
    
    run().catch(console.log)
    
    }
    catch(err){
      console.log(err);
    }

