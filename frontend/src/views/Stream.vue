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
                    <v-progress-circular :model-value="temp.temp" rotate=0 size=180 width=15 :color="indicatorColor" >
                        <span class="text-onSurface font-weight-bold">{{ temp.temp }} C</span> 
                    </v-progress-circular>
                </v-sheet>
                <v-sheet color="surface" class="elevation-0 mb-1 pa-2" max-width=200 width=100% align="center" justify="center">
                    <v-progress-circular :model-value="temp.hi" rotate=0 size=180 width=15 :color="indicatorColor" >
                        <span class="text-onSurface font-weight-bold">{{ temp.hi }} F</span> 
                    </v-progress-circular>
                </v-sheet>
            </v-col>
            <v-col cols="8">
                <v-sheet justify="center">
                <figure class="highcharts-figure">
                    <div id="container"></div>
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
const tempCorF  = ref("");
const hiCorF  = ref("");
const temp = reactive({"temp": 0,"humid": 0, "hi": 0});
const humidity = ref(null); // Chart object 

const tempVal = computed(() => {
    
})

const hiVal = computed(() => {
})

const indicatorColor = computed(()=>{ 
    if  (temp.temp < 15){
        return `rgba(37, 73, 250, 1)`;
    }
    else if (temp.temp < 21){
        return `rgba(50, 250, 180, 1)`;
    }
    else if (temp.temp < 26){
        return `rgba(189, 245, 7, 1)`;
    }
    else if (temp.temp < 30){
        return `rgba(245, 189, 7, 1)`;
    }
    else {
        return `rgba(232, 20, 5, 1)`;
    }
})

const CreateCharts = async () => { 

// CHARTS 
humidity.value = Highcharts.chart('container', { 
    chart: { zoomType: 'x' }, 
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
            name: 'Water', 
            type: 'area', 
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[0] 
        }],     
    }); 

}

// WATCHERS 
watch(payload,(data)=> {  
    if(points.value > 0){  points.value -- ; }
    else{ shift.value = true; }     

    if (tempCorF == "F"){
        temp.temp = data.temperatureFar.toFixed(1);
    }
    else{
        temp.temp = data.temperature.toFixed(1);
    }
    if (hiCorF == "C"){
        temp.hi = data.heatindexCel.toFixed(1);
    }
    else{
        temp.hi = data.heatindex.toFixed(1);
    }

    humidity.value.series[0].addPoint({y:parseFloat(data.humidity.toFixed(2)) ,x: data.timestamp * 1000 }, 
    true,  shift.value);
})


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


</style>
  