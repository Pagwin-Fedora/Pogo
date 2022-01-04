package main

import (
	"fmt"
	"sync"
	"encoding/json"
	"database/sql"
	"unsafe"
	"strings"
	"errors"
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
	msgs := make([]string, len(lines))
	for i, line := range lines {
		tokens := strings.Split(line," ")
		args := tokens[1:len(tokens)]
		var attrBased bool;
		var editDestroy bool;
		var attr string;
		switch attri := tokens[0]; attri {
			case "ITEM":
				attr = "item"
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
				attr = "items"
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
		msg, err := processParsec(attr,editDestroy,args,attrBased,state_obj.db)
		msgs[i] = msg
		if err != nil {
			panic("error oof")
		}
	}
	return C.CString(strings.Join(msgs,"\n"))
}
func processParsec(field string, editDestroy bool, args []string, attrBased bool, db *sql.DB)(string, error){
	var result sql.Result
	var rows *sql.Rows
	var err error
	switch field {
		case "item":
			result, rows, err = processItemQuery(editDestroy,args,db)
			break
		case "items":
			result, rows, err = processItemsQuery(editDestroy,args,db)
			break
		case "progress":
			result, rows, err = processProgressQuery(editDestroy,args,db)
			break
		case "name":
		case "description":
		case "metadata":
			result, rows, err = processAttrQuery(field,editDestroy,args,db)
			break
		case "parents":
		case "children":
			result, rows, err = processMaternalQuery(field, editDestroy, args, db)
			break;
		default:
			return "", errors.New("invalid field")
	}
	if err != nil {
		panic(err)
	}
	if editDestroy {
		return constructModMessage(field, args), nil
	} else {
		//TODO:I need to send messages that indicate state changes that correspond to what the actual internal state is
		return constructDataMessage(field, result, rows), nil
	}

}
func constructDataMessage(field string, result sql.Result, rows *sql.Rows) string {
	if field == "item" && rows != nil {
		val, err := processItemRows(rows)
		if err != nil {
			panic(err)
		}
		//return an items response that provides the new list of all existing items
		 fmt.Sprintf("ITEMS ", )
	}
	else
}
func constructModMessage(field string, args []string) string{
	return strings.ToUpper(field)+strings.Join(args," ")
}
func processItemRows(rows *sql.Rows) int, error {
	defer rows.Close()
	rows.Next()
	var val int
	err := rows.Scan(&val)
	return val, err
}
func processItemQuery(editDestroy bool, args []string, db *sql.DB)(sql.Result, *sql.Rows, error){
			if editDestroy{
				result, err := db.Exec("DELETE FROM pogo_items WHERE id = ?")
				if err != nil {
					return nil,nil, err
				}
				return result, nil, nil
			} else {
				rows, err := db.Query("INSERT INTO pogo_items DEFAULT VALUES RETURNING id")
				if err != nil {
					return nil, nil, err
				}
				return nil, rows, nil
			}
	
}
func processItemsQuery(editDestroy bool, args []string, db *sql.DB)(sql.Result, *sql.Rows, error){
			if editDestroy{
				result, err := db.Exec("DELETE FROM pogo_items WHERE NOT id in ?", args)
				if err != nil {
					return nil, nil, err
				}
				return result, nil, nil
			} else {
				rows, err := db.Query("SELECT id FROM pogo_items")
				if err != nil {
					return nil, nil, err
				}
				return nil, rows, nil
			}
	
}
func processProgressQuery(editDestroy bool, args []string, db *sql.DB)(sql.Result,*sql.Rows, error){
			if editDestroy{
				result, err := db.Exec("SET progress=(?,?) WHERE id=?", args[1], args[2], args[0])
				if err != nil {
					return nil, nil, err
				}
				return result, nil, nil
			} else {
				rows, err := db.Query("SELECT progress FROM pogo_items")
				if err != nil {
					return nil, nil, err
				}
				return nil, rows, nil
			}
	
}
func processAttrQuery(field string, editDestroy bool, args []string, db *sql.DB)(sql.Result, *sql.Rows, error){
			if editDestroy{
				result, err := db.Exec("UPDATE pogo_items SET ? = ? WHERE id = ?", field,strings.Join(args[1:]," "), args[0])
				if err != nil {
					return nil, nil, err
				}
				return result, nil, nil
			} else {
				rows, err := db.Query("SELECT ? FROM pogo_items WHERE id = ?",field,args[0])
				if err != nil {
					return nil, nil, err
				}
				return nil, rows, nil
			}
	
}
func processMaternalQuery(field string, editDestroy bool, args []string, db *sql.DB)(sql.Result, *sql.Rows, error){
			if editDestroy{
				result, err := db.Exec("UPDATE pogo_items SET ? = {?} WHERE id = ?", field, args[0])
				//uuuuuuuuuuuuuuuh
				//attr, args[1:], args[0]
				if err != nil {
					return nil, nil, err
				}
				return result, nil, nil
			} else {
				rows, err := db.Query("SELECT ? FROM pogo_items WHERE id = ?",field,args[0])
				return nil, rows, err
			}
	
}
func main(){}
