/**
 * SA:MP JSON plugin version 1.0.0 by Manuel Schnitzer
 */

native json_decode(json_data[]);
native json_get_int(id, key[]);
native json_get_string(id, key[], value[], value_length);
native Float:json_get_float(id, key[]);
native json_delete(id);
