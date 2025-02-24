<template>
    <v-container fluid align="center" bg-color="surface">
        <v-row justify="center">
            <v-col cols="15">
                <v-card title="Combination" subtitle="Enter your 4 digit passcode">
                    <v-divider></v-divider>
                    
                </v-card>
                <v-btn text="Submit" variant="outlined" color="secondary" flat></v-btn>
            </v-col>
        </v-row>
    </v-container>
</template>

<script setup>
/** JAVASCRIPT HERE */

// IMPORTS
import { ref,reactive,watch ,onMounted,onBeforeUnmount,computed } from "vue";  
import { useRoute ,useRouter } from "vue-router";
 
 
// VARIABLES
const router      = useRouter();
const route       = useRoute();  


// FUNCTIONS
onMounted(()=>{
    // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
});


onBeforeUnmount(()=>{
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
});


</script>


<style scoped>
/** CSS STYLE HERE */


</style>
  