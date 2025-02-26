<template>
    <v-container fluid bg-color="surface">
        <v-row max-width=1200px class="pd-1">
            <v-col cols="5">
                <v-sheet class="pd-2" height=250>
                    <p>Enter date range for Analysis</p>
                    <v-divider></v-divider>
                    <v-text-field flat type="Date" density="compact" variant="solo-inverted" style="max-width: 300px;" label="Start date" v-model="start" class="mr-5"></v-text-field>
                    <v-text-field flat type="Date" density="compact" variant="solo-inverted" style="max-width: 300px;" label="End date" v-model="end"></v-text-field>
                    <v-spacer></v-spacer>
                    <v-btn @click="updateCharts(); updateCards();" text="Analyze" color="primary" variant="tonal"></v-btn>
                </v-sheet>
            </v-col>
            <v-col cols="3" align="center">
                <v-card title="Average" subtitle="For the selected period" width=250 variant="outlined" color="primary" density="compact" rounded="lg">
                <v-card-item align="center">
                    <span class="text-h1 text-primary font-weight-bold">{{ reserves.avg }}</span>
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
const reserves    = reactive({"avg":0} );
const reserveChart= ref(null); // Chart object
const heightScatter   = ref(null);


const CreateCharts = async () => { 

// WATER MANAGEMENT CHART 
reserveChart.value = Highcharts.chart('container', { 
    chart: { zoomType: 'x' }, 
    title: { text: 'Water Management Analysis', align: 'left' }, 

    yAxis: {  
        title: { text: 'Air Temperature & Heat Index' , style:{color:'#000000'}},
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
    title: { text: 'Height and Water Level Correlation Analysis', align: 'left' }, 


    yAxis: {  
        title: { text: 'Height' , style:{color:'#000000'}},
        labels: { format: '{value} in' }  
    },

    xAxis: { 
        title: { text: 'Water Height', style:{color:'#000000'} }, 
        labels: { format: '{value} in' }  
    },

    tooltip: { pointFormat: 'Height: {point.x} in <br/> Water Height: {point.y} in' }, 


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
        let startDate = new Date(start.value).getTime() / 1000; 
        let endDate   = new Date(end.value).getTime() / 1000;  

        // Fetch data from backend 
        const data = await AppStore.getAllInRange(startDate,endDate); 

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
        let startDate = new Date(start.value).getTime() / 1000; 
        let endDate   = new Date(end.value).getTime() / 1000;  

         // 2. Fetch data from backend by calling the API functions  
        const wtr = await AppStore.getReserveAVG(startDate,endDate); 

        reserves.avg = wtr[0].avg.toFixed(1);        
    }
}





// FUNCTIONS
onMounted(()=>{
    // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
    CreateCharts(); 
});


onBeforeUnmount(()=>{
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
});


</script>


<style scoped>
/** CSS STYLE HERE */
Figure { 
    border: 2px solid black; 
} 

</style>
  