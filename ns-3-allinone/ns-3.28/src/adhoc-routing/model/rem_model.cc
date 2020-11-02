#include "rem_model.h"
#include <math.h>
#include <cmath>

// Add a data point to this modelParameters.and call fit model
void PredictionModel::addDataPoint(double value, double time)
{
    if (REM_DEBUG)
        cout << "[DEBUG]: New data point added to model "
                << time << ", " << value << endl;

    // add new value and verify vector size
    mvData.push_back(value);
    if(mvData.size() > RSS_DATA_LIMIT)
    {
        mvData.erase(mvData.begin());
    }

    this->mvTimes.push_back(time);
    if(mvTimes.size() > RSS_DATA_LIMIT)
    {
        mvTimes.erase(mvTimes.begin());
    }

    // add new weighted average point
    if (mvData.size() < 2)
    {
        // add value as initial weighted average
        this->mvWeightedAverages.push_back(value);
    }
    else
    {
        double wAvg = this->mvWeightedAverages.back() * (1 - this->alpha)
        + value * this->alpha;

        // add point to weighted averages and verify vector length
        this->mvWeightedAverages.push_back(wAvg);
        if(mvWeightedAverages.size() > RSS_DATA_LIMIT)
        {
            mvWeightedAverages.erase(mvWeightedAverages.begin());
        }
    }

    if(windowSize < RSS_DATA_LIMIT) {
        windowSize++;
    }

    // check if initializing
    if (mvData.size() < INIT_COUNT)
    {
        if (REM_DEBUG)
            cout << "[DEBUG]: Not enough data to fit model " << endl;
        return;
    }
    else if (mvData.size() == INIT_COUNT)
    {
        // generate a fit
        fitModel();
        return;
    }

    // modelParameters.past initialization
    // compare this point to the expected point
    if (withinExpectedValueRange(time, value))
    {
        difCount = 0;
        this->timeToLive = DEFAULT_TTL;
        // TODO: Do we want this fitModel? What happens when a point is good?
        //fitModel();
    }
    else
    {
        // point is not within expected range
        difCount++;

        if (difCount >= MAX_DIFF_COUNT)
        {
            // too many bad data points in a row
            adaptModel(); // reset difCount
        }
        else
        {
            // check old modelParameters.to see if a mistake was made in updating the model
            compareToPreviousModel();
        }
    }
}

// calculate sigma for the given data set
void PredictionModel::calculateDeviations(const vector<double>& times, const vector<double>& values)
{
	// Passing and running through vectors could be costly...
	// cout << "[JAD] PredictionModel::calculateDeviations\n";
    // get sigma values
    double totalDeviation = 0;
    int n = times.size();
    // calculate deviation on current windowSize
    // TODO: Check that this is windowSize iterations
    for (int i = 0; i < n; i++)
    {
        totalDeviation += std::abs(float(values[i] - this->getDataPoint(times[i])));
    }

    // calculate sigma
    modelParameters.sigma = totalDeviation / n;

    // TODO: Add to error array?
}

 // check if the calculated value is a valid prediction
bool PredictionModel::withinExpectedValueRange(double time, double value)
{
    // if the point lies within valid value return true

    double expected = this->getDataPoint(time);
    double deviation = std::abs(float(value - expected));
    double threshold = modelParameters.sigma * NUM_DEVIATIONS;

    if (threshold < MIN_DEVIATION)
        threshold = MIN_DEVIATION;

    if (REM_DEBUG)
    {
        cout << "[DEBUG]: Expected: " << expected << " vs. Measured: " << value << endl;
        cout << "[DEBUG]: Error threshold: " << threshold << " vs. Deviation: " << deviation << endl;
    }

    if ( deviation > MAX_THRESH_DIFF)// || value < RSS_OUT_OF_RANGE || expected < RSS_OUT_OF_RANGE)
    {
        if (REM_DEBUG)
            cout << "[DEBUG]: BAD: Threshold error" << endl;

        // zero tolerance for threshold error, make new model
        this->state = ModelState::BAD;
        difCount = MAX_DIFF_COUNT;
        return false;
    }
    else if (deviation > threshold)
    {
        if (REM_DEBUG)
            cout << "[DEBUG]: UNSTABLE: Standard deviation error" << endl;

        // standard deviation error
        this->state = ModelState::UNSTABLE;
        return false;
    }
    else if (deviation > modelParameters.sigma && ((deviation < 0 && lastDeviation < 0) || (deviation > 0 && lastDeviation > 0)))
    {
        if (REM_DEBUG)
            cout << "[DEBUG]: UNSTABLE: Bad trend error" << endl;

        // trend ERROR
        this->state = ModelState::UNSTABLE;
        return false;
    }

    this->state = ModelState::STABLE;
    return true;
}

// compare current modelParameters.to previous modelParameters. and check for mistake
// return true if old modelParameters.is better
bool PredictionModel::compareToPreviousModel()
{
  // TODO: Implement

  return false;
}

// adapt the current modelParameters.for the latest measured data in window
void PredictionModel::adaptModel()
{
    if (REM_DEBUG)
        cout << "[DEBUG]: Adapting model to new data" << endl;

    // TODO: Consider going back to old modelParameters.
    // store current modelParameters.in old model
    this->oldModel = modelParameters;

    // build a new modelParameters.and last 'bad data'
    this->windowSize = MAX_DIFF_COUNT;

    // get data for regression
    vector<double> newData;
    vector<double> newTimes;
    vector<double> newAvgs;

    for (uint16_t i = min(mvData.size(), mvTimes.size()) - windowSize; i < min(mvData.size(), mvTimes.size()); i++)
    {
        if (newAvgs.size() > 0)
            newAvgs.push_back(mvData[i - 1] * (1 - alpha) + mvData[i] * alpha);
        else
            newAvgs.push_back(mvData[i]);

        newData.push_back(mvData[i]);
        newTimes.push_back(mvTimes[i]);
    }

    this->performRegression(newTimes, newData, newAvgs);
    this->compareToPreviousModel();

    // create modelParameters.packet
    // broadcast packet
    modelParameters.timeToLive = DEFAULT_TTL;

    // set model needs to be broadcasted and NetworkMonitor will take care of it
    needsToBeBroadcasted = true;
}

/* NS3-TODO: 
void PredictionModel::setParentNode(Ptr<Node> parent)
{
  this->parentNode = parent;
  this->ownerId = parent->GetId();
}
*/

/* Class BatteryModel */
BatteryModel::BatteryModel()
{
    this->alphaTimeout = 1.0;
    this->modelParameters.type = BATTERY;
    this->MODEL_TYPE = BATTERY;
}

void BatteryModel::initialize()
{
    if (BATTERY_DEBUG)
        cout << "[DEBUG]: Initializing battery model for node " << getStringFromIp(this->ownerIp) << endl;

    needsToBeBroadcasted = false;

    // start monitoring in ns3 discrete simulator

    // NS3-TODO: Abstract battery gathering?
//    Simulator::Schedule(Seconds(this->alphaTimeout), &GatherInitialBatteryData, this);
}

double BatteryModel::getDataPoint(double time)
{
    if (this->state == ModelState::STABLE)
        return modelParameters.mu * time + modelParameters.beta;
    else 
        return -1.0;
}

void BatteryModel::fitModel()
{
//  if (windowSize >= INIT_COUNT)
//   cout << "[DEBUG]: Fitting battery modelParameters. << endl;

    performRegression(mvTimes, mvData, mvWeightedAverages);

    modelParameters.timeToLive = DEFAULT_TTL;

    // this model needs to be broadcasted!
    needsToBeBroadcasted = true;
}

void BatteryModel::performRegression(const vector<double>& times, const vector<double>& values, const vector<double>& wAvgs)
{
	// This doesn't get called much
	// cout << "[JAD] BatteryModel::performRegression\n";

    // perform linear regression on data and store it in model
    double x = 0;
    double y = 0;
    double xx = 0;
    double yy = 0;
    double xy = 0;

    int n = times.size();

    for (int i = 0; i < n; i++)
    {
        x += times[i];
        y += values[i];
        xx += times[i] * times[i];
        yy += values[i] * values[i];
        xy += times[i] * values[i];
    }

    double b = (y*xx - x*xy)/(n*xx - x*x);
    double m = (n*xy - x*y)/(n*xx - x*x);

    modelParameters.mu = m;
    modelParameters.beta = b;

    if (BATTERY_DEBUG)
        cout << "[DEBUG]: New Battery model y = " << m << "x + " << b << endl;

    this->state = ModelState::STABLE;

    this->calculateDeviations(times, values);
}

/* Class RssModel */
RssModel::RssModel()
{
    this->modelParameters.type = RSS;
    this->MODEL_TYPE = RSS;
}

void RssModel::initialize()
{
    if (RSS_DEBUG)
        cout << "[DEBUG]: Initializing local rss model for node " << this->ownerIp << " with node " << this->pairIp << endl;

    needsToBeBroadcasted = false;
}

double RssModel::getDataPoint(double time)
{
    if (this->state == ModelState::STABLE)
        return modelParameters.mu * log(time + 10) + modelParameters.beta;
    else 
        return RSS_OUT_OF_RANGE;
}

void RssModel::fitModel()
{
  //if (windowSize >= INIT_COUNT)
   // cout << "[DEBUG]: Fitting rss modelParameters. << endl;

    performRegression(mvTimes, mvData, mvWeightedAverages);

    modelParameters.timeToLive = DEFAULT_TTL;
    // NS3-TODO: Abstarct???
//    Simulator::Schedule(Seconds(0.00001), &BroadcastModel, this);
}

void RssModel::performRegression(const vector<double>& times, const vector<double>& values, const vector<double>& wAvgs)
{
	// This gets called a lot!
	// cout << "[JAD] RssModel::performRegression\n";

    // perform linear regression on data and store it in model
    double x = 0;
    double y = 0;
    double xx = 0;
    double yy = 0;
    double xy = 0;

    int n = times.size();

    for (int i = 0; i < n; i++)
    {
        // linearize input data
        double time = log(times[i] + 10);
        x += time;
        y += values[i];
        xx += time * time;
        yy += values[i]* values[i];
        xy += time * values[i];
    }

    double b = (y*xx - x*xy)/(n*xx - x*x);
    double m = (n*xy - x*y)/(n*xx - x*x);

    modelParameters.mu = m;
    modelParameters.beta = b;

    if (RSS_DEBUG) 
        cout << "[DEBUG]: New RSS model y = " << m << "x + " << b << endl;

    this->calculateDeviations(times, values);
}

REMModelPacket PredictionModel::createREMModelPacket()
{
    REMModelPacket model;

    model.type = this->MODEL_TYPE;
    model.parentIp = this->ownerIp;
    model.timeToLive = this->DEFAULT_TTL;
    model.mu = this->modelParameters.mu;
    model.beta = this->modelParameters.beta;
    model.sigma = this->modelParameters.sigma;
    model.pairIp = 0;
    
    if (this->MODEL_TYPE == ModelType::RSS)
    {
        model.pairIp = this->pairIp; 
    }

    if (MONITOR_DEBUG)
        cout << "[DEBUG]: REM model packet created from node " << getStringFromIp(model.parentIp) << endl;

    return model;
}
