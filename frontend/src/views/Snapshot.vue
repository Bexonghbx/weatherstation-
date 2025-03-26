<template>
    <v-container fluid bg-color="surface">
        <v-row max-width=1200px class="pd-1">
            <v-col cols="3" align="center">
                <v-card title="Tempurature" subtitle="For current Snapshot" width=250 variant="outlined" color="primary" density="compact" rounded="lg">
                <v-card-item align="center">
                    <span class="text-h1 text-primary font-weight-bold">{{ aver.t }}</span>
                </v-card-item>
                </v-card>
            </v-col>
            <v-col cols="3" align="center">
                <v-card title="Humidity" subtitle="For current Snapshot" width=250 variant="outlined" color="primary" density="compact" rounded="lg">
                <v-card-item align="center">
                    <span class="text-h1 text-primary font-weight-bold">{{ aver.h }}</span>
                </v-card-item>
                </v-card>
            </v-col>
            <v-col cols="3" align="center">
                <v-card title="Soil Mositure" subtitle="For current Snapshot" width=250 variant="outlined" color="primary" density="compact" rounded="lg">
                <v-card-item align="center">
                    <span class="text-h1 text-primary font-weight-bold">{{ aver.s }}</span>
                </v-card-item>
                </v-card>
            </v-col>
            <v-col cols="3" align="center">
                <v-card title="Air Pressure" subtitle="For current Snapshot" width=250 variant="outlined" color="primary" density="compact" rounded="lg">
                <v-card-item align="center">
                    <span class="text-h1 text-primary font-weight-bold">{{ aver.a }}</span>
                </v-card-item>
                </v-card>
            </v-col>
        </v-row>
        <v-row max-width=1200px>
            <v-col cols="12">
                <figure class="highcharts-figure">
                    <div id="container"></div>
                </figure>
            </v-col>
            <v-col cols="12">
                <figure class="highcharts-figure">
                    <div id="container0"></div>
                </figure>
            </v-col>
        </v-row>
    </v-container>
</template>

<script setup>
/** JAVASCRIPT HERE */

// IMPORTS
import { ref,reactive,onMounted,onBeforeUnmount} from "vue";  
import { useRoute ,useRouter } from "vue-router";
import { useAppStore } from "@/store/appStore"; 


// Highcharts, Load the exporting module and Initialize exporting module. 
import Highcharts from 'highcharts'; 
import more from 'highcharts/highcharts-more'; 
import Exporting from 'highcharts/modules/exporting'; 
Exporting(Highcharts);  
more(Highcharts); 
 
 
// VARIABLES
const router      = useRouter();
const route       = useRoute();  
const AppStore    = useAppStore(); 
const start       = ref("");  
const end         = ref("");
const aver    = reactive({"t":0, "h":0, "s":0, "a":0} );
const reserveChart= ref(null); // Chart object
const heightScatter   = ref(null);
const intervalId = ref(null);


const startAutoRefresh = async () => {
    if (!intervalId.value) {
        intervalId.value = setInterval(() => {
            location.reload();
        }, 5000);
    }
}; //Restart every 15 seconds

const stopAutoRefresh = async () => {
    if (intervalId.value) {
        clearInterval(intervalId.value);
        intervalId.value = null;
    }
};


const CreateCharts = async () => { 

// WATER MANAGEMENT CHART 
reserveChart.value = Highcharts.chart('container', { 
    chart: { zoomType: 'x' }, 
    title: { text: 'Temperature v Humidity v Heat Index', align: 'left' }, 

    yAxis: {  
        title: { text: 'Air Temperature, Humidity, & Heat Index' , style:{color:'#000000'}},
        labels: { format: '{value} Gal' }  
    },

    xAxis: { 
        type: 'datetime', 
        title: { text: 'Time', style:{color:'#000000'} }, 
    },

    tooltip: { shared:true, pointFormat: 'Time: {point.x} <br/> Reserve: {point.y} Gal' }, 

    series: [
        {
            name: 'Reserve', 
            type: 'spline', 
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[0] 
        }],     
    });
    

heightScatter.value = Highcharts.chart('container0', { 
    chart: { zoomType: 'x' }, 
    title: { text: 'Altitude v Air Pressure Analysis', align: 'left' }, 


    yAxis: {  
        title: { text: 'Altitude' , style:{color:'#000000'}},
        labels: { format: '{value} in' }  
    },

    xAxis: { 
        title: { text: 'Air Pressure', style:{color:'#000000'} }, 
        labels: { format: '{value} in' }  
    },

    tooltip: { pointFormat: 'Altitude: {point.x} in <br/> Air Pressure: {point.y} in' }, 


    series: [
        {
            name: 'Analysis', 
            type: 'scatter', 
            data: [],
            turboThreshold: 0,
            color: Highcharts.getOptions().colors[0] 
        }],     
    });

}; 

const updateCharts = async ()=>{ 
    if(!!start.value && !!end.value){  
        // Convert output from Textfield components to 10 digit timestamps
        // Fetch data from backend 
        const data = await AppStore.getSnapshot(); 

        // Create arrays for each plot 
        let water   = [];  
        let height     = [];  

        // Iterate through data variable and transform object to format recognized by highcharts 
        data.forEach(row => { 
            water.push({"x": row.timestamp * 1000, "y": parseFloat(row.reserve.toFixed(2))  }); 
            height.push({ "x": parseFloat(row.waterheight.toFixed(2)),"y": parseFloat(row.radar.toFixed(2)) }); 
        });

        // Add data to Water Analysis chart 
        reserveChart.value.series[0].setData(water);
        // Add data to Height Scatter chart 
        heightScatter.value.series[0].setData(height); 
    }
}

const updateCards = async ()=>{ 
    if(!!start.value && !!end.value){  
        // 1. Convert start and end dates collected fron TextFields to 10 digit timestamps 

         // 2. Fetch data from backend by calling the API functions  
        const limit = await AppStore.getSnapshot(); 

        aver.t = limit[0].temperature.toFixed(1); 
        aver.h = limit[0].humidity.toFixed(1); 
        aver.s = limit[0].soilmoisture.toFixed(1); 
        aver.a = limit[0].airpressure.toFixed(1); 
        console.log("reached");
    }
}





// FUNCTIONS
onMounted(()=>{
    // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
    CreateCharts(); 
    updateCards();
    updateCharts();

    startAutoRefresh();

});


onBeforeUnmount(()=>{
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
    stopAutoRefresh();
});

</script>


<style scoped>
/** CSS STYLE HERE */
Figure { 
    border: 2px solid black; 
} 

</style>
  