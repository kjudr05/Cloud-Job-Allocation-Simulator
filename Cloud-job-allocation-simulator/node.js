const express = require('express');
const { spawn } = require('child_process');
const bodyParser = require('body-parser');
const cors = require('cors');

const app = express();
const PORT = 3000;

app.use(cors());
app.use(bodyParser.json());
app.use(express.static('public'));

app.get('/', (req, res) => {
  res.send('Cloud Scheduler API is running.');
});

// POST /schedule
app.post('/schedule', (req, res) => {
  const { jobs, servers, algorithm } = req.body;

  if (!jobs || !servers || !algorithm) {
    return res.status(400).json({ error: 'Missing required data' });
  }

  const scheduler = spawn('./scheduler'); // compiled C program

  let output = '';

  scheduler.stdout.on('data', (data) => {
    output += data.toString();
  });

  scheduler.stderr.on('data', (data) => {
    console.error(`stderr: ${data}`);
  });

  scheduler.on('close', (code) => {
    if (code !== 0) {
      return res.status(500).json({ error: 'Scheduler failed' });
    }
    try {
      const result = JSON.parse(output);
      res.json(result);
    } catch (err) {
      res.status(500).json({ error: 'Invalid output format' });
    }
  });

  // Send data to C program via stdin
  scheduler.stdin.write(`${jobs.length} ${servers.length} ${algorithm}\n`);
  jobs.forEach(job => {
    scheduler.stdin.write(`${job.id} ${job.cpu} ${job.memory} ${job.storage}\n`);
  });
  servers.forEach(server => {
    scheduler.stdin.write(`${server.id} ${server.cpu} ${server.memory} ${server.storage}\n`);
  });
  scheduler.stdin.end();
});

app.listen(PORT, () => {
  console.log(`Server listening on http://localhost:${PORT}`);
});
