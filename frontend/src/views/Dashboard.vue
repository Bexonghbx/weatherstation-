<template>
    <v-container>
        <v-row>
            <v-col>
                <v-sheet>
                    <v-slider  direction="vertical" label="Height(cm)" color="green" track-color="blue" :max="78" :min="0" :step="0.5" readonly :model-value="payload.height" thumb-label="always" align="start"></v-slider>
                </v-sheet>
            </v-col>
            <v-col cols=9>
                <figure class="highcharts-figure">
                    <div id="container"></div>
                </figure>
            </v-col>
        </v-row>
        <v-row>
            <v-col cols="8">
                <v-sheet justify="center">
                <figure class="highcharts-figure">
                    <div id="container1"></div>
                </figure> 
                </v-sheet>
            </v-col>
            <v-col cols="4">
                <v-sheet>
                    <v-card align="center">
                        <figure>
                            <div id="fluid-meter"></div>
                        </figure> 
                    </v-card>
                </v-sheet>
            </v-col>
        </v-row>
    </v-container>
</template>

<script setup>
/** JAVASCRIPT HERE */

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
const points  = ref(600); // Specify the quantity of points to be shown on the live graph simultaneously. 
const shift = ref(false); // Delete a point from the left side and append a new point to the right side of the graph.
const { payload, payloadTopic } = storeToRefs(Mqtt);
const waterReserves = ref(null); // Chart object 
const waterCurrent = ref(null); // Chart object 



const CreateCharts = async () => { 

// WATER CHARTS 
waterReserves.value = Highcharts.chart('container', { 
    chart: { zoomType: 'x' }, 
    title: { text: 'Water Reserves', align: 'left' }, 

    yAxis: {  
        title: { text: 'Water level' , style:{color:'#000000'}},
        labels: { format: '{value} Gal' }  
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

waterCurrent.value = Highcharts.chart('container1', {
    chart: {
        type: 'gauge',
        plotBackgroundColor: null,
        plotBackgroundImage: null,
        plotBorderWidth: 0,
        plotShadow: false,
        height: '80%'
    },

    title: { text: 'Water Reserves', align: 'left' },

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
        max: 1000,
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
            from: 600,
            to: 1000,
            color: '#55BF3B', // green
            thickness: 20,
            borderRadius: '50%'
        }, {
            from: 0,
            to: 200,
            color: '#DF5353', // red
            thickness: 20,
            borderRadius: '50%'
        }, {
            from: 200,
            to: 600,
            color: '#DDDF0D', // yellow
            thickness: 20
        }]
    },

    series: [{
        name: 'Reserves',
        data: [0],
        tooltip: {
            valueSuffix: ' Gal'
        },
        dataLabels: {
            format: '{y} Gal',
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

var fm = new FluidMeter();
    fm.init({
    targetContainer: document.getElementById("fluid-meter"),
    fillPercentage: 0,
    options: {
        fontFamily: "Raleway",
        drawPercentageSign: true,
        drawBubbles: true,
        size: 300,
        borderWidth: 19,
        backgroundColor: "#e2e2e2",
        foregroundColor: "#fafafa",
        foregroundFluidLayer: {
        fillStyle: "purple",
        angularSpeed: 100,
        maxAmplitude: 12,
        frequency: 30,
        horizontalSpeed: -150,
        },
        backgroundFluidLayer: {
        fillStyle: "pink",
        angularSpeed: 100,
        maxAmplitude: 9,
        frequency: 30,
        horizontalSpeed: 150,
        },
    },
    });


}

// WATCHERS 
watch(payload,(data)=> {  
    if(points.value > 0){  points.value -- ; }
    else{ shift.value = true; }     

    waterReserves.value.series[0].addPoint({y:parseFloat(data.reserves.toFixed(2)) ,x: data.timestamp * 1000 }, 
    true,  shift.value); 
    
    waterCurrent.value.series[0].data[0] = parseFloat(data.reserves.toFixed());  

    fm.setPercentage(parseFloat(data.percentage.toFixed()));
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
/** .v-slider{
    width: 30px;
}*/

Figure { 
    border: 2px solid black; 
} 
</style>
  