FROM node:latest
RUN npm install
COPY . /usr/app
COPY ../package.json /usr/app
WORKDIR /usr/app
EXPOSE 5672
ENTRYPOINT [ "node", "subscriber" ]