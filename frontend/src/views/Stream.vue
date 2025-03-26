<template>
    <v-container>
        <v-row>
            <v-col cols=2>
                <v-sheet>
                    <v-slider direction="vertical" label="Altitude(m)" color="blue" track-color="white" :max="1000" :min="0" :step="1" readonly :model-value="payload.altitude" thumb-label="always" align="start"></v-slider>
                </v-sheet>
            </v-col>
            <v-col cols=2 >
                <v-sheet color="surface" class="elevation-0 mb-1 pa-2" max-width=200 width=100% align="center" justify="center">
                    <v-progress-circular :model-value="temp.temp" rotate=0 size=100 width=15 :color="indicatorColor" >
                        <span class="text-onSurface font-weight-bold">{{ temp.temp }}</span> 
                    </v-progress-circular>
                    <v-divider></v-divider>
                    <v-progress-circular :model-value="temp.hi" rotate=0 size=100 width=15 :color="indicatorColor" >
                        <span class="text-onSurface font-weight-bold">{{ temp.hi }}</span> 
                    </v-progress-circular>
                    <v-spacer></v-spacer>
                    <v-btn @click = "cfConvert();" text="Change Units" color="primary" variant="tonal"></v-btn>
                </v-sheet>
            </v-col>
            <v-col cols="8">
                <v-sheet justify="center">
                <figure class="highcharts-figure">
                    <div id="container0"></div>
                </figure> 
                </v-sheet>
            </v-col>
        </v-row>
        <v-row>
            <v-col cols="10">
                <v-sheet justify="center">
                <figure class="highcharts-figure soil">
                    <div id="container1"></div>
                </figure> 
                </v-sheet>
            </v-col>
            <v-col cols="10">
                <v-sheet justify="center">
                <figure class="highcharts-figure soil">
                    <div id="container2"></div>
                </figure> 
                </v-sheet>
            </v-col>
        </v-row>

    </v-container>
</template>

<script setup>
/** JAVASCRIPT HERE */

// IMPORTS
// IMPORTS
import { ref,reactive,watch ,onMounted,onBeforeUnmount,computed } from "vue";  
import { useRoute ,useRouter } from "vue-router";
import { useMqttStore } from '@/store/mqttStore'; // Import Mqtt Store 
import { storeToRefs } from "pinia"; 
import Highcharts from 'highcharts'; 
import more from 'highcharts/highcharts-more'; 
import Exporting from 'highcharts/modules/exporting'; 
Exporting(Highcharts);  
more(Highcharts);



 
 
// VARIABLES
const router      = useRouter();
const route       = useRoute();  
const Mqtt        = useMqttStore(); 
const points  = ref(10); // Specify the quantity of points to be shown on the live graph simultaneously. 
const shift = ref(false); // Delete a point from the left side and append a new point to the right side of the graph.
const { payload, payloadTopic } = storeToRefs(Mqtt);
const tempCorF  = reactive({"t": "C"});
const hiCorF  = reactive({"h" : "C"});
const temp = reactive({"temp": '0 C',"humid": 0, "hi": '0 C', "t" : 0});
const humidity = ref(null); // Chart object 
const soilMoisture = ref(null);
const airPressure = ref(null);

const tempVal = computed(() => {
    
})

const hiVal = computed(() => {
})

const indicatorColor = computed(()=>{ 
    if  (temp.t < 25){
        return `rgba(37, 73, 250, 1)`;
    }
    else if (temp.t < 28){
        return `rgba(50, 250, 180, 1)`;
    }
    else if (temp.t < 31){
        return `rgba(189, 245, 7, 1)`;
    }
    else if (temp.t < 34){
        return `rgba(245, 189, 7, 1)`;
    }
    else {
        return `rgba(232, 20, 5, 1)`;
    }
})

const CreateCharts = async () => { 

// CHARTS 
humidity.value = Highcharts.chart('container0', { 
    chart: { zoomType: 'x',
    backgroundColor: {
            patternOptions: {
                pattern: 'image', // Specifies it's an image
                width: 312,
                height: 212
            }
        }
      }, 
    title: { text: 'Humidity', align: 'left' }, 

    yAxis: {  
        title: { text: 'Humidity' , style:{color:'#000000'}},
        labels: { format: '{value} %' }  
    },

    xAxis: { 
        type: 'datetime', 
        title: { text: 'Time', style:{color:'#000000'} }, 
    },

    tooltip: { shared:true, }, 

    series: [
        {
            name: 'Humidity', 
            type: 'spline', 
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[3] 
        }],     
    }); 

soilMoisture.value = Highcharts.chart('container1', { 
    chart: { zoomType: 'x',
    backgroundColor: {
            patternOptions: {
                pattern: 'image', // Specifies it's an image
                width: 512,
                height: 512
            }
        }
     }, 
    title: { text: 'Soil Moisture', align: 'left' }, 

    yAxis: {  
        title: { text: 'Soil Moisture' , style:{color:'#000000'}},
        labels: { format: '{value} %' }  
    },

    xAxis: { 
        type: 'datetime', 
        title: { text: 'Time', style:{color:'#000000'} }, 
    },

    tooltip: { shared:true, }, 

    series: [
        {
            name: 'Moisture', 
            type: 'areaspline', 
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[0] 
        }],     
    }); 

airPressure.value = Highcharts.chart('container2', {
    chart: {
        type: 'gauge',
        plotShadow: false,
        height: '80%'
    },

    title: { text: 'Air Pressure', align: 'left' },

    pane: {
        startAngle: -90,
        endAngle: 89.9,
        background: null,
        center: ['50%', '75%'],
        size: '110%'
    },

    // the value axis
    yAxis: {
        min: 0,
        max: 102,
        tickPixelInterval: 72,
        tickPosition: 'inside',
        tickColor: Highcharts.defaultOptions.chart.backgroundColor || '#FFFFFF',
        tickLength: 20,
        tickWidth: 2,
        minorTickInterval: null,
        labels: {
            distance: 20,
            style: {
                fontSize: '14px'
            }
        },
        lineWidth: 0,
        plotBands: [{
            from: 50,
            to: 100,
            color: '#55BF3B', // green
            thickness: 20,
            borderRadius: '50%'
        }, {
            from: 0,
            to: 6,
            color: '#DF5353', // red
            thickness: 20,
            borderRadius: '50%'
        }, {
            from: 6,
            to: 50,
            color: '#DDDF0D', // yellow
            thickness: 20
        }]
    },

    series: [{
        name: 'Air Pressure',
        data: [0],
        tooltip: {
            valueSuffix: ' kPa'
        },
        dataLabels: {
            format: '{y} kPa',
            borderWidth: 0,
            color: (
                Highcharts.defaultOptions.title &&
                Highcharts.defaultOptions.title.style &&
                Highcharts.defaultOptions.title.style.color
            ) || '#333333',
            style: {
                fontSize: '16px'
            }
        },
        dial: {
            radius: '80%',
            backgroundColor: 'gray',
            baseWidth: 12,
            baseLength: '0%',
            rearLength: '0%'
        },
        pivot: {
            backgroundColor: 'gray',
            radius: 6
        }

    }]

    });
//
}

// WATCHERS 
watch(payload,(data)=> {  
    if(points.value > 0){  points.value -- ; }
    else{ shift.value = true; }     

    if (tempCorF.t == "F"){
        temp.temp = `${data.temperatureFar.toFixed(1)} F`;
    }
    else{
        temp.temp = `${data.temperature.toFixed(1)} C`;
    }
    if (hiCorF.h == "F"){
        temp.hi = `${data.heatindex.toFixed(1)} F`;
    }
    else{
        temp.hi = `${data.heatindexCel.toFixed(1)} C`;
    }

    temp.t = data.heatindexCel.toFixed(1);

    humidity.value.series[0].addPoint({y:parseFloat(data.humidity.toFixed(2)) ,x: data.timestamp * 1000 }, 
    true,  shift.value);

    soilMoisture.value.series[0].addPoint({y:parseFloat(data.soilmoisture.toFixed(2)) ,x: data.timestamp * 1000 }, 
    true,  shift.value);

    airPressure.value.series[0].data[0] = parseFloat(data.airpressure/1000);

    //console.log("Not working");
})

const cfConvert = async ()=>{ 
    if (hiCorF.h === "C"){
        tempCorF.t = "F";
        hiCorF.h = "F";
    }
    else
    {
        tempCorF.t = "C";
        hiCorF.h = "C";
    }
//console.log("Working");


}


// FUNCTIONS
onMounted(()=>{
    // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
    CreateCharts();
    Mqtt.connect();

    setTimeout( ()=>{ 
        Mqtt.subscribe("620148117"); 
    },3000); 
});


onBeforeUnmount(()=>{
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
    Mqtt.unsubcribeAll(); 
});





</script>


<style scoped>
/** CSS STYLE HERE */

#container0 {
    background-image: url('http://localhost:8000/humid.jpeg');
    background-size: cover;
}

#container1 {
    background-image: url('http://localhost:8000/soil_background.jpg');
    background-size: cover;
}


</style>
  