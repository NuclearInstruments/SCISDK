# Simple Register

The register is the most simple configurable element in SciCompiler. A register contains a 32 bit value and can be read or write.

The register support only two simple operation: <b>read</b> and <b>write</b>

A read or write to register is a blocking operation and the execution do not contine till operation is completed.

*Pay attention that the register in SciCompiler can be created as Read Only, Write Only and Read Write. A register, in order to be configured ad read write, must be placed two times in the diagram, one in write and one in read mode, in order to readout the written value*

## Endpoint
The register has a dedicated endpoint 

```
board0:/Registers/<name of the register>
```

if the register is in a design sub-page the address will be
```
board0:/Registers/page1/<name of the register>
```

## Methods

Set a value in the register

```c
SCISDK_DLL_API int SCISDK_SetRegister(char* Path, int value, void* handle);
```

Get a value in the register
```c
SCISDK_DLL_API int SCISDK_GetRegister(char* Path, int*value, void*handle);
```

 - `Path`: the full path of the register
 - `value`: value to be set or value read
 - `handle`: handle to the library

Python, C++, C# and Java uses classes indeed handle is not required

*C++:*
```c++
sdk.SetRegister(Path, value);
sdk.GetRegister(Path, &value);
```

*C Sharp:*
```csharp
sdk.SetRegister(Path, value);
sdk.GetRegister(Path, ref value);
```

*Python:*
```python
err = sdk.SetRegister(Path, value)
err, value = sdk.GetRegister(Path, value)
```

# Example
The following examples uses three register A, B, C. The firmware will calcualte C=A+B (and store the result in C). We set register A and B to 10 and 20 and we read the value of C.

## C
```c
SCISDK_SetRegister("board0:/Registers/A", 10, handle);
SCISDK_SetRegister("board0:/Registers/B", 20, handle);
int value;
SCISDK_GetRegister("board0:/Registers/C", &value, handle);
printf("C=%d\n", value);
```

## C++
```cpp
sdk.SetRegister("board0:/Registers/A", 10);
sdk.SetRegister("board0:/Registers/B", 20);
int value;
sdk.GetRegister("board0:/Registers/C", &value);
printf("C=%d\n", value);
```

## C Sharp
```csharp
sdk.SetRegister("board0:/Registers/A", 10);
sdk.SetRegister("board0:/Registers/B", 20);
int value;
sdk.GetRegister("board0:/Registers/C", ref value);
Console.WriteLine("C={0}", value);
```

## Python
```python
err = sdk.SetRegister("board0:/Registers/A", 10)
err = sdk.SetRegister("board0:/Registers/B", 20)
err, value = sdk.GetRegister("board0:/Registers/C", 0)
print("C={0}\n".format(value))
```

## JAVA
```java
sdk.SetRegister("board0:/Registers/A", 10);
sdk.SetRegister("board0:/Registers/B", 20);
Ref<Integer> value = new Ref<>(0);
sdk.GetRegister("board0:/Registers/C", value);
System.out.println("Register C = (A+B) value is: " + value.get());
```


