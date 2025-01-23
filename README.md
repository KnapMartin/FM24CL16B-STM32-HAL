# FM24CL16B-STM32-HAL
FM24CL16B I2C FRAM library for STM32 with HAL drivers.

# Example

Initialize

```
FM24CL16B fram;
if (FM24CL16B_init(&fram, &hi2c3) != FM24CL16B_OK)
{
    // handle error
}
```

Write

```
if (FM24CL16B_write8(&fram, 0x77, 0x41) != FM24CL16B_OK)
{
    // handle error
}
```

Read

```
uint8_t readData;
if (FM24CL16B_write8(&fram, 0x77, &readData) != FM24CL16B_OK)
{
    // handle error
}
```


# TODO
* I2C ISR error handling
