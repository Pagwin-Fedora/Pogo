package main

import (
	"fmt"
	"sync"
	"encoding/json"
	"database/sql"
	"unsafe"
	"strings"
	_ "github.com/jackc/pgx/v4/stdlib"
)

//#include<stdlib.h>
//#include "../pogo-daemon-api-extern.h"
import "C"

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
//export MessageReceive
func MessageReceive(state *C.void, message *C.char, messageLen C.size_t, returnMessageLen *C.size_t) *C.char{
	state_obj := (*BackendState)(unsafe.Pointer(state))
	go_msg := string((*(*[]byte)(unsafe.Pointer(message)))[:messageLen])
	*returnMessageLen = messageLen
	state_obj.mutex.Lock()
	defer state_obj.mutex.Unlock()
	lines := strings.Split(go_msg,"\n")
	msgs := new([len(lines)]string)
	for i, line := range lines {
		tokens := strings.Split(line," ")
		args := tokens[1:len(tokens)]
		var attrBased bool;
		var editDestroy bool;
		var attr string;
		switch attri := tokens[0]; attri {
			case "ITEM":
				attr = "I"
				attrBased = false
				switch len(args){
					case 0:
						editDestroy = false
						break
					case 1:
						editDestroy = true
						break
					default:
						return C.CString("ERROR to many arguments")
				}
				break
			case "ITEMS":
				attr = "IS"
				attrBased = false
				switch len(args){
					case 0:
						editDestroy = false
						break
					default:
						editDestroy = true
				}
				break
			case "NAME":
			case "DESCRIPTION":
			case "PARENTS":
			case "CHILDREN":
			case "PROGRESS":
			case "METADATA":
				switch len(args){
					case 0:
						return C.CString("ERROR to few arguments")
					case 1:
						editDestroy = false
						break
					default:
						editDestroy = true
						break
				}
				attrBased = true
				attr = strings.ToLower(attri)
				break
			case "ERROR":
				return C.CString("AWK")
			case "AWK":
				return C.CString("")
			default:
				panic("if you see this then you need to setup the default case for the postgres backend")
		}
		msgs[i], err = processMessage(attr,editDestroy,args,db)
		if err != nil {
			panic("error oof")
		}
	}
	return C.CString(strings.Join(msgs,"\n"))
}
//
func processParsec(field string, editDestroy bool, args []string, transaction sql.Db)(string, error){
	query, args, err := constructQuery(atrr,editDestroy,args, db)
	//resp = state_obj.db.Query(query)
	//read into the stdlib Rows object it'll help getting the data out
	
	//query_results := 
	//should probably check for an error before I think about returning
	if editDestroy {
		return message
	} else {
		//I need to send messages that indicate state changes that correspond to what the actual internal state is

	}
	return message
}
//
func constructQuery(field string, editDestroy bool,args []string, transaction sql.Db)(sql.Stmt, []string, error){
	conjoined_args = strings.join(args," ")
	switch field {
		case "I":
			if editDestroy{
				args := new([1]string)
				statement, err := transaction.Prepare("DELETE FROM pogo_items WHERE id = ?"), args, nil
				if err != nil {
					panic("err = nil")
				}
				return statement
			} else {
				args := new([0]string)
				statement, err := transaction.Prepare("INSERT INTO pogo_items DEFAULT VALUES RETURNING id")
				if err != nil {
					return nil, nil, err
				}
				return statement, args, nil
			}
		case "IS":
			if editDestroy{
				statement, err := transaction.Prepare("DELETE FROM pogo_items WHERE NOT id in ?", args)
				if err != nil {
					return nil, nil, err
				}
				return statement, args, nil
			} else {
				statement, err := transaction.Prepare("SELECT id FROM pogo_items")
				if err != nil {
					return nil, nil, err
				}
				rargs := new([0]string)
				return statement, rargs, nil
			}
		case "progress":
			if editDestroy{
				statement, err := transaction.Prepare("PreparePrepareSET progress=(?,?) WHERE id=?", args[1], args[2], args[0])
				if err != nil {
					return nil, nil, err
				}
				rargs := new([3]string)
				rargs[0] = args[1]
				rargs[1] = args[2]
				rargs[2] = args[0]
				return statement, rargs, nil
			} else {
				statement, err := transaction.Prepare("SELECT progress FROM pogo_items")
				if err != nil {
					return nil, nil, err
				}
				rargs := new([0]string)
				return statement, rargs, nil
			}
		case "name":
		case "description":
		case "metadata":
			if editDestroy{
				statement, err := transaction.Prepare("UPDATE pogo_items SET ? = ? WHERE id = ?", attr,strings.join(args[1:]," "), args[0])
				if err != nil {
					return nil, nil, err
				}
				return statement
			} else {
				statement, err := transaction.Prepare("SELECT ? FROM pogo_items")
				if err != nil {
					return nil, nil, err
				}
				rargs := new([1]string)
				rargs[0] = attr
				return statement, rargs, nil
			}
		case "parents":
		case "children":
			if editDestroy{
				statement, err := transaction.Prepare("UPDATE pogo_items SET ? = {?} WHERE id = ?")
				//uuuuuuuuuuuuuuuh
				//attr, args[1:], args[0]
			} else {
				statement, err := transaction.Prepare("SELECT ? FROM pogo_items WHERE id = ?")
				rargs := new([2]string)
				rargs[0] = attr
				rargs[1] = args[0]
				return statement, rargs, nil
			}
	}

}
//why do you have to do this to me go
func main(){}
