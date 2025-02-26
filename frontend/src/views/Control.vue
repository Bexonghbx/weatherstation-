<template>
    <v-container fluid align="center" bg-color="surface">
        <v-row justify="center">
            <v-col cols="15">
                <v-card title="Combination" subtitle="Enter your 4 digit passcode">
                    <v-otp-input :length="4" v-model="passcode"></v-otp-input>
                </v-card>
                <v-btn text="Submit" variant="outlined" color="secondary" flat @click="updatePasscode();"></v-btn>
            </v-col>
        </v-row>
    </v-container>
</template>

<script setup>
/** JAVASCRIPT HERE */

// IMPORTS
import { ref,reactive,watch ,onMounted,onBeforeUnmount,computed } from "vue";  
import { useRoute ,useRouter } from "vue-router";
import { useAppStore } from "@/store/appStore"; 
 
 
// VARIABLES
const router        = useRouter();
const route         = useRoute();  
const passcode      = ref("");
const AppStore    = useAppStore(); 

const updatePasscode = async ()=>{
    let newPasscode = passcode.value;

    await AppStore.updatePasscode(newPasscode);
}

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
  