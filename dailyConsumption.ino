int calibrate = 1; // 0 = Output to Processing Application while 1 = Calibration mode
int loadPin = 0;  // Arduino analog pin to read

// LOAD CELL CALIBRATION
#define LOAD_LOW 0
#define ANALOG_LOW (NUM)
#define LOAD_HIGH (NUM)
#define ANALOG_HIGH (NUM)

static long loadLow = LOAD_LOW; // measured low end load in grammes from good scales
static int analogLow = ANALOG_LOW   ; // analog reading obtained from experiments
static long loadHigh = LOAD_HIGH ; //  obtained high end load in grammes from good scales
static int analogHigh = ANALOG_HIGH ; //  analog reading from load cell for high end test load

// This is used when you change the load cell platform to something else that weighs
// different and the load is no longer on zero. Add an offset to set to zero.
int loadAdjustment = LOAD_LOW;  //  Adjust non loaded load cell to 0




// Values for the analog sample buffer for running average 
static int samplesBuffer = 100;  // Should be the same as the number of samples
int analogSamples[100] = {0};  // Number of analog samples to average and set to 0
int lastSampleIndex = 0; // Last analog sample

long sampleBufferTotal = 0; // The sum of all the samples held in the buffer
long analogSamplesAverage = 0; // Running average of the sum of the buffer

// Results plot or display frequency
long time = 0;  // Set time to mark start of delay


void setup() {

  if (calibrate) {
    Serial.begin(9600);  // Set a slower boadrate for calibration
  } else {
    Serial.begin(115200);  // Set a faster baudrate for Processing Application
  }
}

void loop() {

  //an old value here
  
  //a new value
  
  //subtract
  
  //add sample
  
  //the new value becomes the new old value 
  
  //get average
  
  int loadValue = analogRead(loadPin);  // Get analog read
  int oldestSample = addSample(loadValue);
  
  // Get running average, pass the latest and oldest analog sample reading
 samplesAverage = runningAverage(loadValue, oldestSample);
  
  /*if debug
    if (calibrate) {// print test results
      //Serial.print("Analog pin value: ");Serial.println(loadValue);
      Serial.print("Smoothed analog value: ");Serial.println(samplesAverage);
      Serial.print("Scale load (grammes): ");Serial.println(loadGrams);
      Serial.println(" ");
    } else {  // Output to Processing as such
      Serial.println(loadGrams);
    }*/
    
    time = millis();  // Set time to mark start of delay
  }
}

// Function - running average
long runningAverage(long newSample, long oldSample) {
  // Add new sample and subtract old sample from the samples buffer total
  // then return sample average
  sampleBufferTotal += newSample;  // Add new analog sample
  sampleBufferTotal -= oldSample;  // Subtract oldest analog sample
  return sampleBufferTotal / samplesBuffer;  // Return average analog sample reading
}

// Function - add and remove analog samples from ring buffer, return oldest sample
// The ring buffer is used to keep track of the oldest sample.
int addSample(int newSample) {
  // Add new analog read sample to the ring buffer and return the oldest analog reading
  int oldSample;
  if (lastSampleIndex == samplesBuffer - 1 ) { // Check if end off buffer reached
    oldSample = analogSamples[0];  // Get old analog sample from start of buffer
    analogSamples[0] = newSample;  // Add new analog sample to start of buffer
    lastSampleIndex = 0;  // Record sample index currently working on
  } else {
    lastSampleIndex ++;  // Move to next index
    oldSample = analogSamples[lastSampleIndex];  // Get old analog sample
    analogSamples[lastSampleIndex] = newSample;  // Add new analog sample
  }
  return oldSample;  // Return old analog sample
}
