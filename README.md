hotl (Header-only template library)

`hotl` includes generic hash table, hash set and array list types. It uses the
generic programming technique of [clt](https://github.com/glouw/ctl) (the
type-safe template-like c library).

It defines templates that are used to define concrete types. For example,
an `arl` (array list) type of `int` with default comparator, copy and clean
functions is defined including `<arl.h>` after defining `T`:

```C
#define T int
#include <arl.h>
```

And instance is defined by means of the constructor `ArlOf`:

```C
ArlOf(int) a = (ArlOf(int)){0};
```

Or if you prefer
```C
ArlOf(int)* p = &(ArlOf(int)){0};
```

For example, elements can be appended with `append` or accessed with `at`,
but the functions themselves are return values of macro-functions.

```C
int i = 2;
arlfn(int, append)(&a, &i); //or arlfn(int, append)(p, &i);

int* a0 = arlfn(int,at)(&a,0);
if (a0) *a0 = 3;
```

The macro `arlfn` receives a type `T` and a name `N` and returns the
function for `N` for an `ArlOf(T)`.

# Types
## LipMap (linear probing hash table).

`lipmap` is the hash table. Its constructor is `LipOf` and receives the key
and value types (and optionally a key type hash function, comparator,
copier and cleaner).


```
LipMap:
KeyType, ValueType [, Hash, KeyTypeCmp, KeyTypeCpy, ValueTypeCpy] -> LipMap
```

`Hotl_LipMap_KT`     : the key type (required).
`Hotl_LipMap_VT`     : the value type (required).
`Hotl_LipMap_K_Hash`     : `KeyType* -> size_t`: the hash function  (optional).
`Hotl_LipMap_KT_Cmp` : `KeyType*, KeyType* -> int` the comparison function for values of the key type (optional).
`Hotl_LipMap_KT_Cpy` : `KeyType*, KeyType* -> int` copy function for values of the key type (optional).
`Hotl_LipMap_VT_Cpy` : `ValueType*, ValueType* -> int` copy function for values of the value type (optional).

Default values for parameters are in `hotl.h`:
`Hotl_LipMap_K_Hash`     : `hotl_djb2_k33_with_seed`
`Hotl_LipMap_KT_Cmp` : `hotl_compare_bytes`
`Hotl_LipMap_KT_Cpy` : `hotl_copy_bytes`
`Hotl_LipMap_VT_Cpy` : `hotl_copy_bytes`


Methods:
--------
+ `init`:        LipOf(K,V)*, LipInitArgs -> int
+ `del`:         LipOf(K,V)*, K*          -> int 
+ `clean`:       LipOf(K,V)*              -> int 
+ `get_or_set`:  LipOf(K,V)*, K*, V*      -> V*
+ `get`:         LipOf(K,V)*, K*          -> V*
+ `set`:         LipOf(K,V)*, K*, V*      -> int

## LipSep (linear probing set).

`lipset` is the hash set. Its constructor is `LipOf` and receives the element
type (and optionally its hash function, comparator, copier and cleaner).


```
LipSet:
Type [, Hash, TypeCmp, TypeCpy] -> LipSet
```

`Hotl_LipSet_T`     : the key type (required).
`Hotl_LipSet_Hash`     : `Type* -> size_t`: the hash function  (optional).
`Hotl_LipSep_T_Cmp` : `Type*, Type* -> int` the comparison function for values of the key type (optional).
`Hotl_LipSep_T_Cpy` : `Type*, Type* -> int` copy function for values of the key type (optional).

Default values for parameters are in `hotl.h`:
`Hotl_LipSet_Hash`     : `hotl_djb2_k33_with_seed`
`Hotl_LipSet_T_Cmp` : `hotl_compare_bytes`
`Hotl_LipSet_T_Cpy` : `hotl_copy_bytes`


Methods:
--------
+ `init`  : LipOf(T)*, LipInitArgs -> int
+ `del`   : LipOf(T)*, T*          -> int 
+ `clean` : LipOf(T)*              -> int 
+ `find`  : LipOf(T)*, T*          -> T*
+ `insert`: LipOf(T)*, T*          -> int

## Arl (Array list)

Arl: T, TCmp, TCpy, TClean -> ArlOf(T)

+ `T`:      element type
+ `TCmp`:   element ptr comparator. Default: `hotl_compare_bytes` (`memcmp
  sizeof T`)
+ `TCpy`:   element ptr copy. Default: `hotl_copy_bytes` (`memmove sizeof
  T`).
+ `TClean`: element ptr clean. Default: `(void)`.

Methods:
-------

+ `append`: `ArlOf(T)*, T* -> int`
+ `at`:     `ArlOf(T)*, size_t -> T*`
+ `back`:   `ArlOf(T)* -> T*`
+ `begin`:  `ArlOf(T)* -> T`
+ `clean`:  `ArlOf(T)* -> void`
+ `end`:    `ArlOf(T)* -> T`
+ `find`:   `ArlOf(T)*, T* -> T*`

`ArlOf(T)` can be iterated using `begin` and `end` like in:

```C
for(it = arlfn(int, begin(x)) ; it != arlfn(int, end(x)); ++it) {
    // ...
}
```

# Usage

To define a `LipOf(KT, VT)` we use macros like:


```C
#define KT int
#define VT int
#include <lipmap.h> // LipOf(int,int)
```

The key type default comparator is `memcmp` passing a pointer to a value
of key type and `sizeof(KT)`.

The key type default copier is `memmove` passing a pointer to a value
of key type and `sizeof(KT)`.

The key type default clean does nothing.

To define custom key comparator and copy use `KTCmp` and `KTCpy` like:

```C
#define KT cstr
#define KTCmp cstrp_compare
#define KTCpy cstrp_copy
#define VT int
#define KHash cstrp_hash
#include <lipmap.h> // LipOf(cstr, int)
```


`lip-cstr-size_t.h` defines a `LipOf(char*, size_t)` setting `KTCmp`,
`KTCpy`, `KTClean` and `KHash`.

The `typedef char* cstr` is needed because the macros used by hotl need
their type parameters to be source code strings instead of actual types.

`lip-string-size_t.h` define a `LipOf(string, size_t)` where `string` is
`typedef` for `struct {char*; size_t len;}`.

