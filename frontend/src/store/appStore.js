import {defineStore} from 'pinia'
import {ref} from 'vue'


export const useAppStore =  defineStore('app', ()=>{

    /*  
    The composition API way of defining a Pinia store
    ref() s become state properties
    computed() s become getters
    function() s become actions  
    */ 

    // STATES 
  


    // ACTIONS
    const getAllInRange = async (start,end)=>{ 
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller    = new AbortController(); 
        const signal        = controller.signal; 
        const id            = setTimeout(()=>{controller.abort()},60000); 
        const URL           = `/api/weather/${start}/${end}`;  

        try { 
            const response  = await fetch(URL,{ method: 'GET', signal: signal  }); 
            if (response.ok){
                const data      = await response.json(); 
                let keys        = Object.keys(data); 

                if(keys.includes("status")){ 

                    if(data["status"] == "found" ){ 
                        // console.log(data["data"]  )   
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){
                        console.log("getAllInRange returned no data"); 
                    }
                }
            }
            else{
                const data      = await response.text();
                console.log(data); 
            }
        }
        catch(err){ 
            console.error('getAllInRange error: ', err.message); 
        }
        return [] 
    }

    const getSnapshot = async ()=>{ 
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller    = new AbortController(); 
        const signal        = controller.signal; 
        const id            = setTimeout(()=>{controller.abort()},60000); 
        const URL           = `/api/weather/snapshot`;  

        try { 
            const response  = await fetch(URL,{ method: 'GET', signal: signal  }); 
            if (response.ok){
                const data      = await response.json(); 
                let keys        = Object.keys(data); 

                if(keys.includes("status")){ 

                    if(data["status"] == "found" ){ 
                        // console.log(data["data"]  )   
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){
                        console.log("getSnapshot returned no data"); 
                    }
                }
            }
            else{
                const data      = await response.text();
                console.log(data); 
            }
        }
        catch(err){ 
            console.error('getSnapshot error: ', err.message); 
        }
        return [] 
    }

    const getCelTempAVG = async (start,end)=>{ 
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller    = new AbortController(); 
        const signal        = controller.signal; 
        const id            = setTimeout(()=>{controller.abort()},60000); 
        const URL           = `/api/mmar/temperature/celsius/${start}/${end}`;  

        try { 
            const response  = await fetch(URL,{ method: 'GET', signal: signal  }); 
            if (response.ok){
                const data      = await response.json(); 
                let keys        = Object.keys(data); 

                if(keys.includes("status")){ 

                    if(data["status"] == "found" ){ 
                        // console.log(data["data"]  )   
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){
                        console.log("getCelTempAVG returned no data"); 
                    }
                }
            }
            else{
                const data      = await response.text();
                console.log(data); 
            }
        }
        catch(err){ 
            console.error('getCelTempAVG error: ', err.message); 
        }
        return [] 
    }

    const getCelHeatIndexAVG = async (start,end)=>{ 
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller    = new AbortController(); 
        const signal        = controller.signal; 
        const id            = setTimeout(()=>{controller.abort()},60000); 
        const URL           = `/api/mmar/hi/celsius/${start}/${end}`;  

        try { 
            const response  = await fetch(URL,{ method: 'GET', signal: signal  }); 
            if (response.ok){
                const data      = await response.json(); 
                let keys        = Object.keys(data); 

                if(keys.includes("status")){ 

                    if(data["status"] == "found" ){ 
                        // console.log(data["data"]  )   
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){
                        console.log("getCelHeatIndexAVG returned no data"); 
                    }
                }
            }
            else{
                const data      = await response.text();
                console.log(data); 
            }
        }
        catch(err){ 
            console.error('getCelHeatIndexAVG error: ', err.message); 
        }
        return [] 
    }

    const getFarTempAVG = async (start,end)=>{ 
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller    = new AbortController(); 
        const signal        = controller.signal; 
        const id            = setTimeout(()=>{controller.abort()},60000); 
        const URL           = `/api/mmar/temperature/fahrenheit/${start}/${end}`;  

        try { 
            const response  = await fetch(URL,{ method: 'GET', signal: signal  }); 
            if (response.ok){
                const data      = await response.json(); 
                let keys        = Object.keys(data); 

                if(keys.includes("status")){ 

                    if(data["status"] == "found" ){ 
                        // console.log(data["data"]  )   
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){
                        console.log("getFarTempAVG returned no data"); 
                    }
                }
            }
            else{
                const data      = await response.text();
                console.log(data); 
            }
        }
        catch(err){ 
            console.error('getFarTempAVG error: ', err.message); 
        }
        return [] 
    }

    const getFarHeatIndexAVG = async (start,end)=>{ 
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller    = new AbortController(); 
        const signal        = controller.signal; 
        const id            = setTimeout(()=>{controller.abort()},60000); 
        const URL           = `/api/mmar/hi/fahrenheit/${start}/${end}`;  

        try { 
            const response  = await fetch(URL,{ method: 'GET', signal: signal  }); 
            if (response.ok){
                const data      = await response.json(); 
                let keys        = Object.keys(data); 

                if(keys.includes("status")){ 

                    if(data["status"] == "found" ){ 
                        // console.log(data["data"]  )   
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){
                        console.log("getFarHeatIndexAVG returned no data"); 
                    }
                }
            }
            else{
                const data      = await response.text();
                console.log(data); 
            }
        }
        catch(err){ 
            console.error('getFarHeatIndexAVG error: ', err.message); 
        }
        return [] 
    }

    const getHumidAVG = async (start,end)=>{ 
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller    = new AbortController(); 
        const signal        = controller.signal; 
        const id            = setTimeout(()=>{controller.abort()},60000); 
        const URL           = `/api/mmar/humidity/${start}/${end}`;  

        try { 
            const response  = await fetch(URL,{ method: 'GET', signal: signal  }); 
            if (response.ok){
                const data      = await response.json(); 
                let keys        = Object.keys(data); 

                if(keys.includes("status")){ 

                    if(data["status"] == "found" ){ 
                        // console.log(data["data"]  )   
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){
                        console.log("getHumidAVG returned no data"); 
                    }
                }
            }
            else{
                const data      = await response.text();
                console.log(data); 
            }
        }
        catch(err){ 
            console.error('getHumidAVG error: ', err.message); 
        }
        return [] 
    }

    const getMositAVG = async (start,end)=>{ 
        // FETCH REQUEST WILL TIMEOUT AFTER 20 SECONDS 
        const controller    = new AbortController(); 
        const signal        = controller.signal; 
        const id            = setTimeout(()=>{controller.abort()},60000); 
        const URL           = `/api/mmar/soilmoisture/${start}/${end}`;  

        try { 
            const response  = await fetch(URL,{ method: 'GET', signal: signal  }); 
            if (response.ok){
                const data      = await response.json(); 
                let keys        = Object.keys(data); 

                if(keys.includes("status")){ 

                    if(data["status"] == "found" ){ 
                        // console.log(data["data"]  )   
                        return data["data"]; 
                    }
                    if(data["status"] == "failed"){
                        console.log("getMositAVG returned no data"); 
                    }
                }
            }
            else{
                const data      = await response.text();
                console.log(data); 
            }
        }
        catch(err){ 
            console.error('getMositAVG error: ', err.message); 
        }
        return [] 
    }

   
 
 
    return { 
    // EXPORTS	
    getAllInRange,
    getCelTempAVG,
    getCelHeatIndexAVG,
    getFarTempAVG,
    getFarHeatIndexAVG,
    getHumidAVG,
    getMositAVG,
    getSnapshot,
        
       }
},{ persist: true  });