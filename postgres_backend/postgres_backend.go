package main

import (
	"fmt"
	"sync"
	"encoding/json"
	"database/sql"
	"github.com/lib/pq"
	"unsafe"
);

//#include<stdlib.h>
import "C";

type BackendState struct {
	config BackendConfig
}
type BackendConfig struct {
	host string `json:dbHost`
	port int `json:dbPort`
	user string `json:dbUser`
	password string `json:dbPassword`
	name string `json:dbName`
}
//export initBackend
func initBackend(size C.size_t, config *C.char){
	//this is scary
	//goConfig = C.GoString(config)
	var config BackendConfig
	byte_array := (*[size]byte)(unsafe.Pointer(config))
	json.Unmarshal(byte_array,&config)
	//sslmode=disable worries me
	dbInfo = fmt.SprintF("host=%s port=%d user=%s password=%s dbname=%s sslmode=disable",config.dbHost,config.dbPort,config.dbUser,config.dbPassword,config.dbName)

}
//export MessageReceive
func MessageReceive(){

	fmt.Println("ono");
}
//why do you have to do this to me go
func main(){}
