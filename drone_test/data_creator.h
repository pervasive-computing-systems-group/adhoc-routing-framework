
/**
 * @brief A base class for objects which create fake data to send over the network
 * 
 */
class DataCreator{
public:
    DataCreator(){};

    virtual char* getData() = 0;
};