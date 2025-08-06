# Benewake ROS2 Driver and Publisher

[![ROS2](https://img.shields.io/badge/ROS2-Jazzy-blue.svg)](https://docs.ros.org/en/humble/index.html)
[![License: Apache 2.0](https://img.shields.io/github/license/saltstack/salt)](LICENSE)

> C++ package to integrate Benewake lidar sensors into ROS2

## ✅ Supported Devices (UART)

- [**TF02-Pro**](https://en.benewake.com/TF02Pro/index.html)
- [**TFmini Plus**](https://en.benewake.com/TFminiPlus/index.html)
- [**TFS20-L**](https://en.benewake.com/TFS20L/index.html) *(untested)*

Benewake uses a common serial protocol across its lidar lineup, with minor variations. Most of this driver should be compatible with other models — consult datasheets to confirm.



## Build

Clone the repository inside your ROS2 workspace and build with `colcon`:

```bash
cd ~/ros2_ws/src
git clone <repo_url>
cd ..
colcon build --packages-select benewake_ros2_driver
source install/setup.bash
```

## 🚀 Usage

```bash
ros2 run benewake_ros2_driver node_lidar \
  --ros-args -p port:=/dev/ttyS0 \
             -p baud:=115200 \
             -p period:=0.01 \
             -p model:=tf-02 \
             -p framerate:=100
```

This launches the node on UART1 of the Raspberry Pi with a connected TF-02 lidar. The `framerate` is deliberately set close to `period` to minimize crosstalk.

## Parameters

> **port** : string  
> - **default**: `"/dev/ttyS0"`  
> - serial port where the sensor is connected  

> **baud** : int  
> - **default**: *115200*  
> - baudrate of the serial port  

> **period** : float  
> - **default**: *0.01 (seconds)*  
> - interval at which the frames sent by the sensor are read  

> **model** : string  
> - **default**: *""*  
> - **accepted**: *"tf-02"*, *"tf-mini-plus"*, *"tf-s20l"*  
> - model of the connected lidar. Determines which `Benewake::ADriver` to instantiate.  

> **trigger** : boolean  
> - **default**: *false*  
> - if `true`, the lidar will operate in *trigger mode*, meaning it will only take measurements when instructed by the driver. This can reduce crosstalk between multiple sensors if measurement timing is synchronized. `framerate` is automatically set to 0.  
> - if `false`, the lidar will operate in *free-running mode*, continuously taking and sending measurements. This can yield higher frame rates but may result in crosstalk.  

> **framerate** : int  
> - **default**: 100  
> - **min**: *0*  
> - **max**: *250 to 1000* depending on model  
> - **unit**: Hz  
> - frame rate at which to configure the sensor — determines how fast it captures and sends data  
> - `framerate=0` and `trigger=true` enables *trigger mode*  
> - `framerate=0` and `trigger=false` disables the sensor  

> **target_baud** : int  
> - **default**: *115200*  
> - baudrate to reconfigure the serial port at startup. Allows higher throughput at high frequencies.  
> - ***Warning***: this change persists after the node is shut down, as long as the sensor remains powered.  
> - ***Warning***: not extensively tested  

## Control Modes

### Trigger mode (`trigger=true`)

`framerate` is set to 0, and measurement interval is based on the `period` parameter.

Useful for reducing crosstalk.

### Free-running mode (`trigger=false` and `framerate > 0`)

The sensor measures and sends data at a frequency defined by `framerate`.

Frames are read from the serial interface every `period` seconds.

If `framerate > (1 / period)`, one read operation may decode multiple frames, leading to the publication of multiple messages per cycle. This may cause irregular publishing rates on the **/range** topic.

## Return Signal Strength and Uncertain Values

Blinding values (`strength = 65535`), low confidence measurements (`strength < 100`), or zero distances (`distance = 0`) are published with a distance set to `NaN`.

## Serial Interface

The `Serial.hpp` class defines an API used by the driver to control and access the serial interface.

It uses non-blocking communication (the `O_NONBLOCK` flag is set on the port's `fd`) and operates in **polling** mode with a timeout assigned to each operation.

Multiple `node_lidar` instances cannot share the same port, as it is locked via a `/var/lock/LCK..dev_name` file on initialization.

## Benewake Driver

The `Benewake` namespace defines and implements a set of classes for using Benewake lidars.

### Benewake::frames

- `struct Benewake::frames::DataFrame`: structure used to parse measurement frames sent by the sensors  
- `struct Benewake::frames::CommandFrame`: structure used to construct command frames  
- `struct Benewake::frames::ResponseFrame`: structure used to parse response frames  

### Benewake::ADriver

Abstract class that defines common methods for all supported models (TF-02, TF-mini Plus, TFS20L).

Declares the following pure virtual methods:

```cpp
virtual void setFrameRate(unsigned int rate) const = 0;
virtual std::string getModelName() const = 0;
virtual std::string getFov() const; // Not pure, but usefull to overload and child class
```


The following classes inherit from it:

- `Benewake::TF02Driver`
- `Benewake::TFMiniPlusDriver`
- `Benewake::TFS20LDriver`


Replace `<repo_url>` with the actual URL of the repository.

---

## Contribution

Contributions are welcome! If you want to add support for other Benewake models or improve compatibility, feel free to open an issue or submit a pull request.


---

## License

This project is licensed under the Apache License, Version 2.0.  
You may obtain a copy of the License at:

[http://www.apache.org/licenses/LICENSE-2.0](http://www.apache.org/licenses/LICENSE-2.0)

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an **"AS IS" BASIS**, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
See the License for the specific language governing permissions and limitations under the License.


## 👤 Author

Created by [@Anvently](https://github.com/Anvently) on repo : https://github.com/Anvently/benewake_lidar_ros2_driver/
