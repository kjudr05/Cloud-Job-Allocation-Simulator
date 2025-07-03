# Cloud-job-allocation-simulator

This is the project for simulating the allocation of the jobs/containers under the cloud environment. 
It has a server code written in 'C' language that implements different algorithms like Greedy, Min-Heap, First fit decreasing.
A Node.js middleware that takes the benefit of creating the local server.
A HTML/JS frontend UI connecting the server efficiently.

## To run:

1. Open the terminal, type - ```npm install```
2. Run the server, type - ```nodemon node.js``` , in the terminal
3. Open the url ```localhost:3000``` in your browser

### If Error:

1. Run the command - ```gcc -o scheduler scheduler.c``` , in the terminal

## Requirements:

1. C compiler - gcc
2. Node.js installed
