# FM24CL16B-STM32-HAL
FM24CL16B I2C FRAM library for STM32 with HAL driversin C and C++.

# Example C

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
if (FM24CL16B_read8(&fram, 0x77, &readData) != FM24CL16B_OK)
{
    // handle error
}
```

# Example C++

Initialize

```
FM24CL16B fram(&hi2c);
```

Write

```
if (fram.write(0x66, (uint8_t)0x11) != FM24CL16B::State::Ok)
{
    // handle error
}
```

Read

```
uint8_t readData;
if (fram.write(0x66, &readData) != FM24CL16B::State::Ok)
{
    // handle error
}
```


# TODO
* I2C ISR error handling
