package main

import (
	"fmt"
	"sync"
	"encoding/json"
	"database/sql"
	"unsafe"
	_ "github.com/jackc/pgx/v4/stdlib"
);

//#include<stdlib.h>
//#include "../pogo-daemon-api.h"
import "C";

type BackendState struct {
	mutex sync.Mutex
	config BackendConfig
	db *sql.DB
}
type BackendConfig struct {
	host string `json:dbHost`
	port int `json:dbPort`
	user string `json:dbUser`
	password string `json:dbPassword`
	name string `json:dbName`
}
//export initBackend
func initBackend(size C.size_t, rawConfig *C.char) *C.void{
	//this is scary
	//goConfig = C.GoString(config)
	var config BackendConfig
	//I think this is fine but I'm not sure lol
	byte_array := (*(*[]byte)(unsafe.Pointer(rawConfig)))[:size]

	json.Unmarshal(byte_array,&config)
	URL := fmt.Sprintf("postgres://%s:%s@%s:%d/%s",config.user,config.password,config.host,config.port,config.name)
	db, err := sql.Open("pgx", URL)
	if err != nil {
		fmt.Println("oh no backend didn't initialize don't know how to actually deal with this so a panic is probably imminent somewhere")
	}
	if db.Ping() != nil {
		fmt.Println("ping failed")
	}
	return (*C.void)(unsafe.Pointer(&BackendState {sync.Mutex{},config,db}))
}
type 
//export MessageReceive
func MessageReceive(state *C.void, message *C.char, messageLen C.size_t, returnMessageLen) *C.char{

}
//why do you have to do this to me go
func main(){}
