int calibrate = 1; // 0 = Output to Processing Application while 1 = Calibration mode
int loadPin = 0;  // Arduino analog pin to read

// LOAD CELL CALIBRATION
#define LOAD_LOW 0  //obtained low end load in grammes from good scales
#define ANALOG_LOW (NUM) //analog reading obtained from experiments
#define LOAD_HIGH (NUM) //obtained high end load in grammes from good scales
#define ANALOG_HIGH (NUM) //analog reading from load cell for high end test load

static long loadLow = LOAD_LOW; 
static int analogLow = ANALOG_LOW; 
static long loadHigh = LOAD_HIGH ; 
static int analogHigh = ANALOG_HIGH ; 


//This is to avoid measurement bias and adjust no load to zero
int loadAdjustment = LOAD_LOW;  
int initLoad = 2000; //assume the user always buys 2kg milk

// Values for the analog sample buffer for running average 
static int samplesBuffer = 14;  // 2 wks
int analogSamples[14] = {0};  // all init-ed as 0
int lastSampleIndex = 0; // Last analog sample

long sampleBufferTotal = 0; // The sum of all the samples held in the buffer
long analogSamplesAverage = 0; // Running average of the sum of the buffer


void setup() {

  if (calibrate) {
    Serial.begin(9600);  // Set a slower boadrate for calibration
  } else {
    Serial.begin(115200);  // Set a faster baudrate for Processing Application
  }
}

void loop() {

  //an old value here
  int initValue
  //a new value
  int loadValue = analogRead(loadPin);  // Get analog read
  //subtract
  int sampleValue = initValue - loadValue;
  //add sample
   int oldestSample = addSample(sampleValue);
  //the new value becomes the new old value 
  initValue = loadValue;
  //get average
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
