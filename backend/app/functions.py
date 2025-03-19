 #!/usr/bin/python3


#################################################################################################################################################
#                                                    CLASSES CONTAINING ALL THE APP FUNCTIONS                                                                                                    #
#################################################################################################################################################


class DB:

    def __init__(self,Config):

        from math import floor
        from os import getcwd
        from os.path import join
        from json import loads, dumps, dump
        from datetime import timedelta, datetime, timezone 
        from pymongo import MongoClient , errors, ReturnDocument
        from urllib import parse
        from urllib.request import  urlopen 
        from bson.objectid import ObjectId  
       
      
        self.Config                         = Config
        self.getcwd                         = getcwd
        self.join                           = join 
        self.floor                      	= floor 
        self.loads                      	= loads
        self.dumps                      	= dumps
        self.dump                       	= dump  
        self.datetime                       = datetime
        self.ObjectId                       = ObjectId 
        self.server			                = Config.DB_SERVER
        self.port			                = Config.DB_PORT
        self.username                   	= parse.quote_plus(Config.DB_USERNAME)
        self.password                   	= parse.quote_plus(Config.DB_PASSWORD)
        self.remoteMongo                	= MongoClient
        self.ReturnDocument                 = ReturnDocument
        self.PyMongoError               	= errors.PyMongoError
        self.BulkWriteError             	= errors.BulkWriteError  
        self.tls                            = False # MUST SET TO TRUE IN PRODUCTION


    def __del__(self):
            # Delete class instance to free resources
            pass
 


    ###############################
    #  WEATHER STATION FUNCTIONS  #
    ###############################
    
    # INSERTS DATA IN TO THE WEATHER COLLECTION
    def weatherUpdate(self,data):
        try:
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = remotedb.ELET2415.weather.insert_one(data)
        except Exception as e:
            msg = str(e)
            if "duplicate" not in msg:
                print("addUpdate error ",msg)
            return False
        else:                  
            return True

    
    #  GATHERS ALL DATA WITHIN A GIVEN TIMEFRAME AND RETURNS IT AS A LIST.
    def getAllInRange(self,start, end):
        try:
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.weather.find({'timestamp':{'$gte':int(start),'$lte':int(end)}},{'_id':0}).sort([('timestamp',1)]))
        except Exception as e:
            msg = str(e)
            print("getAllInRange error ",msg)            
        else:                  
            return result


    # 3. CREATE A FUNCTION TO COMPUTE THE ARITHMETIC AVERAGE ON THE 'reserve' FEILED/VARIABLE, USING ALL DOCUMENTS FOUND BETWEEN SPECIFIED START AND END TIMESTAMPS. RETURNS A LIST WITH A SINGLE OBJECT INSIDE
    
    
    
    # FINDS THE AVERAGE HUMIDITY OF THE AIR BETWEEN TWO DATES AND RETURNS THEM IN A LIST
    def humidityMMAR(self,start, end):
        try:
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.weather.aggregate([{'$match':{'timestamp':{'$gte':int(start),'$lte':int(end)}}},{'$group':{'_id': '','humidity':{'$push':"$$ROOT.humidity"}}},{'$project':{'max':{'$max':'$humidity'},'min':{'$min':'$humidity'},'avg':{'$avg':'$humidity'},'range':{'$subtract':[{'$max':'$humidity'},{'$min':'$humidity'}]}}}]))
        except Exception as e:
            msg = str(e)
            print("humidityMMAR error ",msg)            
        else:                  
            return result

    # FINDS THE AVERAGE HUMIDITY OF THE AIR BETWEEN TWO DATES AND RETURNS THEM IN A LIST
    def soilMoistureMMAR(self,start, end):
        try:
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.weather.aggregate([{'$match':{'timestamp':{'$gte':int(start),'$lte':int(end)}}},{'$group':{'_id': '','moisture':{'$push':"$$ROOT.soilmoisture"}}},{'$project':{'max':{'$max':'$moisture'},'min':{'$min':'$moisture'},'avg':{'$avg':'$moisture'},'range':{'$subtract':[{'$max':'$moisture'},{'$min':'$moisture'}]}}}]))
        except Exception as e:
            msg = str(e)
            print("soilMoistureMMAR error ",msg)            
        else:                  
            return result
        
    # FINDS THE AVERAGE AIR TEMPERATURE IN FAHRENHEIT AND CELSIUS BETWEEN TWO DATES AND RETURNS THEM IN A LIST
    def celTemperatureMMAR(self,start, end):
        try:
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.weather.aggregate( [{'$match':{'timestamp':{'$gte':int(start),'$lte':int(end)}}},{'$group':{'_id': "",'temperature':{'$push':"$$ROOT.temperature"}}},{'$project':{'max':{'$max':'$temperature'},'min':{'$min':'$temperature'},'avg':{'$avg':'$temperature'},'range':{'$subtract':[{'$max':'$temperature'},{'$min':'$temperature'}]}}}]))
        except Exception as e:
            msg = str(e)
            print("celTemperatureMMAR error ",msg)            
        else:                  
            return result
    
    # FINDS THE AVERAGE AIR TEMPERATURE IN FAHRENHEIT AND CELSIUS BETWEEN TWO DATES AND RETURNS THEM IN A LIST
    def farTemperatureMMAR(self,start, end):
        try:
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.weather.aggregate( [{'$match':{'timestamp':{'$gte':int(start),'$lte':int(end)}}},{'$group':{'_id': '','temperature':{'$push':"$$ROOT.temperatureFar"}}},{'$project':{'max':{'$max':'$temperature'},'min':{'$min':'$temperature'},'avg':{'$avg':'$temperature'},'range':{'$subtract':[{'$max':'$temperature'},{'$min':'$temperature'}]}}}]))
        except Exception as e:
            msg = str(e)
            print("farTemperatureMMAR error ",msg)            
        else:                  
            return result

    # FINDS THE AVERAGE HEAT INDEX IN FAHRENHEIT AND CELSIUS BETWEEN TWO DATES AND RETURNS THEM IN A LIST
    def farHeatIndexMMAR(self,start, end):
        try:
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.weather.aggregate( [{'$match':{'timestamp':{'$gte':int(start),'$lte':int(end)}}},{'$group':{'_id': "",'heatindex':{'$push':"$$ROOT.heatindex"}}},{'$project':{'max':{'$max':'$heatindex'},'min':{'$min':'$heatindex'},'avg':{'$avg':'$heatindex'},'range':{'$subtract':[{'$max':'$heatindex'},{'$min':'$heatindex'}]}}}]))
        except Exception as e:
            msg = str(e)
            print("farHeatIndexMMAR error ",msg)            
        else:                  
            return result
    
    # FINDS THE AVERAGE HEAT INDEX IN FAHRENHEIT AND CELSIUS BETWEEN TWO DATES AND RETURNS THEM IN A LIST
    def celHeatIndexMMAR(self,start, end):
        try:
            remotedb 	= self.remoteMongo('mongodb://%s:%s@%s:%s' % (self.username, self.password,self.server,self.port), tls=self.tls)
            result      = list(remotedb.ELET2415.weather.aggregate( [{'$match':{'timestamp':{'$gte':int(start),'$lte':int(end)}}},{'$group':{'_id': '','heatindex':{'$push':"$$ROOT.heatindexCel"}}},{'$project':{'max':{'$max':'$heatindex'},'min':{'$min':'$heatindex'},'avg':{'$avg':'$heatindex'},'range':{'$subtract':[{'$max':'$heatindex'},{'$min':'$heatindex'}]}}}]))
        except Exception as e:
            msg = str(e)
            print("celHeatIndexMMAR error ",msg)            
        else:                  
            return result

   
    
    


   



def main():
    from config import Config
    from time import time, ctime, sleep
    from math import floor
    from datetime import datetime, timedelta
    one = DB(Config)
 
 
    start = time() 
    end = time()
    print(f"completed in: {end - start} seconds")
    
if __name__ == '__main__':
    main()


    
