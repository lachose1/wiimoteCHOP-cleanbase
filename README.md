# Introduction to custom C++ CHOP in Touchdesigner
## Introduction
### What are we gonna do today
* 10 to 10h30 : Setting up the tools
* 10h30 to 11 : Compiling our first basic DLL
* Small 5 mins break
* 11 to 13h30 : Integrating our first custom detector (Wiimote)

### About me
#### My social medias
* [LinkedIn](https://www.linkedin.com/in/hugolaliberte/)
* [Facebook](https://www.facebook.com/lachose1)
* [GitHub](https://github.com/lachose1)
* [Email : hugo@ottomata.com](mailto:hugo@ottomata.com)

#### Small bio
I am a creative coder from Montreal.

I cofounded [Ottomata](http://ottomata.com) studio in 2017 but have been working in the interactive media domain since 2016.

Before working in interactive media I have been a lead developer in a couple of startups.

I started programming at 12 and have a background in electrical engineering which helps a lot when you are playing with new gadgets.

### About this workshop
As we all know, TouchDesigner is a very powerful tool for interactive/media art. I consider it as a central hub to control many inputs and dispatch interactive content to many outputs. However, there will come a time where you will probably need to implement a new tool or detector that insn't natively supported in TouchDesigner and this is why being able to use the powerful C++ Custom OPs is a must. Today, we only have 3:30 which means we will only have the time to focus and go in-depth on the custom CHOP subject, but using the workflow I will present you today, you should be able to understand most of the other OPs by yourself.

### How we are going to proceed
For the first part of the workshop, we will be using all of TouchDesigner's built in codebase to understand the basics and get quickly started.

For the second part of the workshop, we will be using the codebase I shared with you and inspire ourselves from exemple files I included.

I see this workshop as very interactive so don't hesitate to stop me at any time in order to ask a question or clarify a concept.

### Great documentation
* [Official TD C++ doc](https://docs.derivative.ca/Write_a_CPlusPlus_Plugin)
* [The C++ Bible](http://www.cplusplus.com/reference/)

## Setting-up our tools
### Visual Studio
Install [Visual Studio Community Edition](https://visualstudio.microsoft.com/downloads/)

During installation, select these features (can also be installed afterwards by navigating to “Tools -> Get Tools and Features ):
* In `Desktop development with C++` install :
  * C++ (v142) Universal Windows Platform Tool
  * Windows 10 SDK (10.0.17763.0)
  * C++ (v142) Universal Windows Platform Tool
* In `Desktop development with C++` install :
  * .NET Framework 4.6.1 SDK

### Finding the demonstration code
![](https://i.imgur.com/OLYJxQZ.png)

And then you will strike gold : 
* CHOP
* CPUMemoryTOP
* CudaTOP
* DAT
* OpenGLTOP
* SimpleShapesSOP

Copy the code from the CHOP folder to a new location and setup your codebase as your used to (setting up git, moving the code, etc.).

### Cleaning and renaming the codebase
You will notice that the project is already named CPlusPlusCHOPExample and we might wanna change that. However, in Visual Studio, it is not as straightforward as it might look so let's go over the required steps.

1. Open up the solution : `CPlusPlusCHOPExample.sln`
2. If you get the retargeting project window, just accept the settings to retarget
3. Right-click the project and rename it
4. Right-click the solution and rename it
5. In the properties page (f4 if it is closed) : Change the (name) and the Root Namespace
6. Rename CPlusPlusCHOPExample.h and CPlusPlusCHOPExample.cpp to your object desired name
7. You will also need to rename the object you are incuding in both files
8. Rebuild solution just to make sure nothing is broken (`Build -> Rebuild Solution`)
9. Right-click the project and select "Remove". Confirm saving any changed files.
10. Close Visual Studio and inside your project folder rename both the .vcxproj and the .vcxproj.user
11. Open the solution and right-click to add back the .vcxproj

### Setting up Windows
One handy little trick while developing custom c++ OPs is that if we wanna see c++ console output, we can create the env variable TOUCH_TEXT_CONSOLE=1 and then using std::cout, we can output data inside of the debugger console.

We are ready to start!

### What is a DLL
Basically, it is a library of functions and procedures that can be used by another program that can be linked to the process when it is executed.

For instance, we could create a DLL containing functions that knows how to handle connections to a specific Lidar and use this in our program.

The advantage is that we can dynamically load our TouchDesigner specific DLL to access all these powerful functions that we built.

You can load a DLL in TouchDesigner by drag and droping the DLL or creating a custom OP and loading the specific DLL inside `Plugin Path`.

### Looking at the code structure
You can go ahead and take a look at your main object .h file so we can overlook the important methods. Most of the important methods and properties can be found inside two files : 
* CPlusPlus_Common.h
* CHOP_CPlusPlusBase.h

I often take a look at these files to better plan what part of the object I want to use.

## Compiling our first DLL
If everything went fine, we should be able to compile the example code right now so let’s go ahead and compile it.

The compilation should succeed, but the DLL execution should fail, cause we can’t run a DLL alone so let's add a link the TouchDesigner.exe to our project so that when we start debugging, TouchDesigner will be launched with our DLL attached to ease the debugging process.

Open your project settings and inside the Debugging section, in Command. Add a direct path to TouchDesigner.exe `C:\Program Files\Derivative\TouchDesigner099\bin\TouchDesigner099.exe`

### Modifying the general information of the OP
Search for the FillCHOPPluginInfo function and it should be pretty self-explanatory. Over there you can modify the Name, author, email, etc. associated with the plugin.

### Core function ::execute()
Take a look at `::execute()`. This is the core function that outputs information to TouchDesigner and is where we will start most of our work.

### Hands-on 1 : Adding a new Toggle
Let's say we want to add a toggle that adds all the channels together whenever we click the Toggle button. It is already of a small function from the Math CHOP, but it should be a great starting point.

#### Step 1 : modifying ::setupParameters()
I suggest you take a look at method `::setupParameters()`. You can also take a look inside `CPlusPlus_Common.h` at the class `OP_ParameterManager` to get a little more explanation.

<details><summary>Click to show solution</summary>
<p>

```cpp
{
    OP_NumericParameter    np;

    np.name = "Combine";
    np.label = "Combine";
    np.defaultValues[0] = 0.0;

    OP_ParAppendResult res = manager->appendToggle(np);
    assert(res == OP_ParAppendResult::Success);
}
```

</p>
</details>

#### Step 2 : modifying ::getOutputInfo()
Since we are modifying the output, we need to return true for method `::getOutputInfo()` if we wanna set the channel settings manually so let's go ahead and modify the settings as we like.

<details><summary>Click to show solution</summary>
<p>

```cpp
bool combineChans = inputs->getParInt("Combine");
if (combineChans)
{
    info->numChannels = 1;
    return true;
}
```

</p>
</details>

#### Step 3 : coding the logic inside of ::execute()
Now that the toggle exists, let's go ahead and modify what we output based on the toggle state.

<details><summary>Click to show solution</summary>
<p>

```cpp
const OP_CHOPInput* cinput = inputs->getInputCHOP(0);
int numChans = cinput->numChannels;
int ind = 0;
for (int j = 0; j < output->numSamples; j++)
{
    float total = 0;
    for (int i = 0; i < numChans; i++)
    {
        total += float(cinput->getChannelData(i)[ind] * scale);
    }
    ind++;
    ind = ind % cinput->numSamples;
    output->channels[0][j] = total;
}
```

</p>
</details>

#### Step 4 : handling multiple inputs
![](https://i.imgur.com/uCNKDeB.png)
This logic works well, but as you might have noticed, it only works for the first input connected to the CHOP. The rest of the inputs are being ignored. Let's go ahead and try to fix that. Don’t forget to modify the `::getOutputInfo()` to output the correct number of chans.

<details><summary>Click to show solution</summary>
<p>

```cpp
for (int k = 0; k < output->numChannels; k++)
{
    const OP_CHOPInput* cinput = inputs->getInputCHOP(k);
    int numChans = cinput->numChannels;
    int ind = 0;

    for (int j = 0; j < output->numSamples; j++)
    {
        float total = 0;
        for (int i = 0; i < numChans; i++)
        {
            total += float(cinput->getChannelData(i)[ind] * scale);
        }
        ind++;
        // Make sure we don't read past the end of the CHOP input
        ind = ind % cinput->numSamples;
        output->channels[k][j] = total;
        myTotalChans += total;
    }
}
```

</p>
</details>

#### Step 5 : Renaming the chans
Compiling this should work, but there will still be only one chan output inside of TouchDesigner. This is because the function which names the chan keeps overwritting the name of all the chans with `chan1`. Let's take a look at `::getChannelName()` to get an idea. Right now, the same name is returned for all the chans which means that we keep overwritting the older chan with the newer one. Let's modify it.

<details><summary>Click to show solution</summary>
<p>

```cpp
std::string chanName = "chan" + std::to_string(index);
name->setString(chanName.c_str());
```

</p>
</details>

#### Step 6 : Outputting Info DAT and Info CHOP
By now I think you should get the idea. One final important thing we can have a look at is how to output information inside of Info DAT or Info CHOP

Let’s create an object parameter called `float myTotalChans`; and just add a number to it. After that we can have a look at the two methods `::getInfoCHOPChan` and `::getInfoDATEntries` which are pretty much self-explanatory.

<details><summary>Click to show solution</summary>
<p>

```cpp
std::string chanName = "chan" + std::to_string(index);
name->setString(chanName.c_str());
```

</p>
</details>

### Let's take a small break before creating our first real object

## Integrating our first custom detector (Wiimote)
### The logic behind integrating a new detector in TouchDesigner
Whenever I have to integrate something new in TouchDesigner, I observe the same workflow.
1. First of all, I test the tool inside its own SDK. Pretty often, these tools come with some examples or demo software that you can run just to familiarize yourself with the actual tool.
2. I search and organize the libraries I will need in order to connect with the tool. For instance, I might need to write some network code to connect to a Sick Lidar through TCP which requires some core Windows API libraries. I might need to learn about a specific protocol and see how to communicate with it.
3. When I have these libraries ready, I write a standalone cmd application that I can run just to test if I am able to connect with the detector without TouchDesigner.
4. When I have a small program working, I proceed to transfer it inside of a TouchDesigner custom OP

Since we don't have a lot of time today, I basically did steps 1 to 3 for you today. So we are going to start at step 4. I wanted to have a basic tool we can integrate in TouchDesigner so I thought about the Wiimote. There is a pretty great C++ implementation that I found (at step 2) which is called [wiiuse](https://github.com/wiiuse/wiiuse) which is cross-platform and let's you easily talk to a wiimote with your OS so I figured we could implement it in our custom CHOP. I downloaded the source code and compiled a DLL as well as a .lib that we can use in our codebase. You can find the compiled DLL in the folder I gave you inside of `libs\x64`. I also included an example wiiuse code file that we can look at to understand how the library works. It is located inside the `3-wiiuse-dll-example` folder. But first of all, let's connect the wiimote to your PC

### Connecting the Wiimote to your PC
In order to connect the wiimote to your PC, I created a small executable file  which is located inside `1-wiipair-executable`. One by one, you should run this executable and follow the onscreen instructions which are very straightforward. We should asolutely do it one at a time since it is bluetooth and we don't wanna pair our PC with the wrong wiimote. I am gonna go ahead and do it myself as an example.

### Hands-on 2 : Integrating our first detector
#### Step 1 : Looking at the codebase
You can take a look inside the WiimoteCHOP folder, I created a starting codebase for you. You will only need to verify that the argument correctly points to your TouchDesigner.exe file like we did earlier so that when you compile, you will correctly have TouchDesigner opening up. You will also see two new files `WiimoteConnector.cpp` and `WiimoteConnector.h` which represents the object we will use inside of our CHOP to communicate with the wiimote.

#### Step 2 : Linking the libraries
Open the project settings and take a look at `C/C++ -> General`. You should see that I've already added a directory to `Additional Include Directories` which points to `$(SolutionDir)includes`. This folder contains wiiuse.h so that we can use the wiiuse.lib in our program. Afterwards, take a look at `Linker -> General`. Once again, you should see that `Additional Include Directories` contains a link to `$(SolutionDir)libs\x64`. This folder contains the wiiuse.lib and wiiuse.dll that we want to implement. Normally, those two parameters don't contain these links. Whenever you will integrate your own libraries, remember to correctly set these parameters, otherwise you will be unable to compile anything. Speaking of which, let's try and compile the codebase to see if an empty shell works. 

#### Step 3 : Pointing to our first object
We will start be creating an pointer to a `WiimoteConnector` object inside our `WiimoteChop`. Then, we will want it to call the `::update` method everytime it executes, so we can tell the Wiimote to poll with new data. Finally, we might want to output the Wiimote status in a DAT so that we can make sure everything is fine.

<details><summary>Click to show solution</summary>
<p>

```cpp
if (index == 2)
{
    // Set the value for the first column
    entries->values[0]->setString("Wiimote Status");
    // Set the value for the second column
#ifdef _WIN32
    std::string currentStatus = _myWiimote->getCurrentStatus();
#else // macOS
    snprintf(tempBuffer, sizeof(tempBuffer), "%g", myOffset);
#endif
    entries->values[1]->setString(currentStatus.c_str());
}
```

</p>
</details>

#### Step 4 : Toggling the Wiimote connection on and off
Next, we will want to initialize the Wiimote connection with a Toggle, so that if, for instance, the bluetooth is off, we will get an error message and we can try again. First of all, let’s get rid of the unnecessary parameters.

Then we can add a parameter to our WiimoteConnector object to know if the wiimote is actually connected or not. We will also need a parameter for the CHOP to know if it needs to retry the connection, something that remembers the last state of the toggle to compare it.

<details><summary>Click to show solution</summary>
<p>

```cpp
myExecuteCount++;
_myWiimote->update();

bool isWiimoteOn = inputs->getParInt("Wiimote");
if (!_lastWiimoteStatus && isWiimoteOn) //If the last status of the toggle was off and now it is on, attempt connection
{
    _myWiimote->connect();
}
for (int i = 0; i < output->numChannels; i++)
{
    for (int j = 0; j < output->numSamples; j++)
    {
        output->channels[i][j] = float(0.0);
    }
}

_lastWiimoteStatus = isWiimoteOn;
```

</p>
</details>

#### Step 5 : Creating the connection script for the wiimote
Now let’s create the connection script. We will modify the `::connect()` method as well as the `::update()` so that we can see if we are able to connect to the detector. Let's have a look at the example file located in `3-wiiuse-dll-example` for inspiration. As we write it, we might realize that it would be helpful to have an error message also inside of the DAT. We can add it if we want. For the moment I am just gonna output inside of the console any errors. Do not forget to transfer a copy of wiiuse.dll to your debug folder, otherwise the custom chop will stop working at this moment.

<details><summary>Click to show solution</summary>
<p>

```cpp
//Initialization code
int found, connected;
_myStatus = "Connecting";

/*
 *    Initialize an array of wiimote objects.
 *
 *    The parameter is the number of wiimotes I want to create.
 */
_wiimotes = wiiuse_init(MAX_WIIMOTES);

/*
 *    Find wiimote devices
 *
 *    Now we need to find some wiimotes.
 *    Give the function the wiimote array we created, and tell it there
 *    are MAX_WIIMOTES wiimotes we are interested in.
 *
 *    Set the timeout to be 5 seconds.
 *
 *    This will return the number of actual wiimotes that are in discovery mode.
 */
found = wiiuse_find(_wiimotes, MAX_WIIMOTES, 5);
if (!found)
{
    _isConnected = false;
    _attemptedConnection = true;
    std::cout << "No wiimotes found.\n";
    return;
}

/*
 *    Connect to the wiimotes
 *
 *    Now that we found some wiimotes, connect to them.
 *    Give the function the wiimote array and the number
 *    of wiimote devices we found.
 *
 *    This will return the number of established connections to the found wiimotes.
 */
connected = wiiuse_connect(_wiimotes, MAX_WIIMOTES);
if (connected)
{
    _isConnected = true;
    std::cout << "Connected to " << connected << " wiimotes (of " << found << " found).\n";

    wiiuse_set_leds(_wiimotes[0], WIIMOTE_LED_1);
    wiiuse_rumble(_wiimotes[0], 1);
    wiiuse_motion_sensing(_wiimotes[0], 1); //Enable accelerometer
    Sleep(200);
    wiiuse_rumble(_wiimotes[0], 0);
}
else
{
    _isConnected = false;
    _attemptedConnection = true;
    std::cout <<  "Failed to connect to any wiimote.\n";
    return;
}
```

</p>
</details>

#### Step 6 : Polling the wiimote for its data
Once again, going back to `3-wiiuse-dll-example`, we can find inspiration on how to implement a polling logic. There are multiple parameters we might want to poll for instance the state of the buttons or the direction it is pointing. For now, let's just focus on getting the accelerometer data. Let's put the bulk of the logic inside of the `::update()` method as well as copy the necessary functions from the example script.

<details><summary>Click to show solution</summary>
<p>

```cpp
while (any_wiimote_connected(_wiimotes, MAX_WIIMOTES)) {
    if (wiiuse_poll(_wiimotes, MAX_WIIMOTES)) {
        /*
            *    This happens if something happened on any wiimote.
            *    So go through each one and check if anything happened.
            */
        int i = 0;
        for (; i < MAX_WIIMOTES; ++i) {
            switch (_wiimotes[i]->event) {
            case WIIUSE_EVENT:
                /* a generic event occurred */
                handle_event(_wiimotes[i]);
                break;

            case WIIUSE_STATUS:
                /* a status event occurred */
                handle_ctrl_status(_wiimotes[i]);
                break;

            case WIIUSE_DISCONNECT:
            case WIIUSE_UNEXPECTED_DISCONNECT:
                /* the wiimote disconnected */
                handle_disconnect(_wiimotes[i]);
                break;

            case WIIUSE_READ_DATA:
                /*
                    *    Data we requested to read was returned.
                    *    Take a look at wiimotes[i]->read_req
                    *    for the data.
                    */
                break;

            case WIIUSE_NUNCHUK_INSERTED:
                /*
                    *    a nunchuk was inserted
                    *    This is a good place to set any nunchuk specific
                    *    threshold values.  By default they are the same
                    *    as the wiimote.
                    */
                    /* wiiuse_set_nunchuk_orient_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 90.0f); */
                    /* wiiuse_set_nunchuk_accel_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 100); */
                //printf("Nunchuk inserted.\n");
                break;

            case WIIUSE_CLASSIC_CTRL_INSERTED:
                //printf("Classic controller inserted.\n");
                break;

            case WIIUSE_WII_BOARD_CTRL_INSERTED:
                //printf("Balance board controller inserted.\n");
                break;

            case WIIUSE_GUITAR_HERO_3_CTRL_INSERTED:
                /* some expansion was inserted */
                handle_ctrl_status(_wiimotes[i]);
                //printf("Guitar Hero 3 controller inserted.\n");
                break;

            case WIIUSE_MOTION_PLUS_ACTIVATED:
                //printf("Motion+ was activated\n");
                break;

            case WIIUSE_NUNCHUK_REMOVED:
            case WIIUSE_CLASSIC_CTRL_REMOVED:
            case WIIUSE_GUITAR_HERO_3_CTRL_REMOVED:
            case WIIUSE_WII_BOARD_CTRL_REMOVED:
            case WIIUSE_MOTION_PLUS_REMOVED:
                /* some expansion was removed */
                handle_ctrl_status(_wiimotes[i]);
                //printf("An expansion was removed.\n");
                break;

            default:
                break;
            }
        }
    }
}
```

</p>
</details>

#### Step 7 : Multithreading the polling logic
Once again, we hit a problem. The code seems to freeze when we connect to the wiimote. If you take a close look at the logic we just created, it is an infinite while loop. Since the custom C++ chop runs inside the main thread, it basically blocks the whole TouchDesigner process indefinitely. We could only poke the wiimote everytime we update the CHOP (for instance every frame). However, the wiimote code seems to be asynchronous, meaning that it is the wiimote that sends its updated data whenever it wants, so we need to always be listening for updates. The solution to this problem is to put our polling logic inside its own thread. Do not worry, it is not that hard to get started. Let's start be including the threading libraries we need `#include <thread>` and `#include <functional>`.

<details><summary>Click to show solution</summary>
<p>

```cpp
while (any_wiimote_connected(_wiimotes, MAX_WIIMOTES)) {
    if (!_isConnected) //Important so that we exit the loop when the toggle is turned off
        return;
    if (wiiuse_poll(_wiimotes, MAX_WIIMOTES)) {
        /*
            *    This happens if something happened on any wiimote.
            *    So go through each one and check if anything happened.
            */
        int i = 0;
        for (; i < MAX_WIIMOTES; ++i) {
            switch (_wiimotes[i]->event) {
            case WIIUSE_EVENT:
                /* a generic event occurred */
                handle_event(_wiimotes[i]);
                break;

            case WIIUSE_STATUS:
                /* a status event occurred */
                handle_ctrl_status(_wiimotes[i]);
                break;

            case WIIUSE_DISCONNECT:
            case WIIUSE_UNEXPECTED_DISCONNECT:
                /* the wiimote disconnected */
                handle_disconnect(_wiimotes[i]);
                break;

            case WIIUSE_READ_DATA:
                /*
                    *    Data we requested to read was returned.
                    *    Take a look at wiimotes[i]->read_req
                    *    for the data.
                    */
                break;

            case WIIUSE_NUNCHUK_INSERTED:
                /*
                    *    a nunchuk was inserted
                    *    This is a good place to set any nunchuk specific
                    *    threshold values.  By default they are the same
                    *    as the wiimote.
                    */
                    /* wiiuse_set_nunchuk_orient_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 90.0f); */
                    /* wiiuse_set_nunchuk_accel_threshold((struct nunchuk_t*)&wiimotes[i]->exp.nunchuk, 100); */
                //printf("Nunchuk inserted.\n");
                break;

            case WIIUSE_CLASSIC_CTRL_INSERTED:
                //printf("Classic controller inserted.\n");
                break;

            case WIIUSE_WII_BOARD_CTRL_INSERTED:
                //printf("Balance board controller inserted.\n");
                break;

            case WIIUSE_GUITAR_HERO_3_CTRL_INSERTED:
                /* some expansion was inserted */
                handle_ctrl_status(_wiimotes[i]);
                //printf("Guitar Hero 3 controller inserted.\n");
                break;

            case WIIUSE_MOTION_PLUS_ACTIVATED:
                //printf("Motion+ was activated\n");
                break;

            case WIIUSE_NUNCHUK_REMOVED:
            case WIIUSE_CLASSIC_CTRL_REMOVED:
            case WIIUSE_GUITAR_HERO_3_CTRL_REMOVED:
            case WIIUSE_WII_BOARD_CTRL_REMOVED:
            case WIIUSE_MOTION_PLUS_REMOVED:
                /* some expansion was removed */
                handle_ctrl_status(_wiimotes[i]);
                //printf("An expansion was removed.\n");
                break;

            default:
                break;
            }
        }
    }
}
```

</p>
</details>

#### Step 8 : Gracefully closing our thread
Now that the polling logic is in its own thread, we need to make sure that whenever we leave TouchDesigner or when we toggle the wiimote off we correctly close the polling thread. This is called thread joining. Let's create our own `::disconnect()` method.

<details><summary>Click to show solution</summary>
<p>

```cpp
wiiuse_cleanup(_wiimotes, MAX_WIIMOTES);
if (_wiimoteThread.joinable())
{
    _isConnected = false;
    _attemptedConnection = false;
    _wiimoteThread.join();
    std::cout << "Thread Closed";
    //_lidarThread.detach();
    //_lidarThread.~thread();
    //std::terminate();
}
```

</p>
</details>

#### Step 9 : Outputting the data inside of chans
The last thing we will need to do is output the data of the accelerometer inside of TouchDesigner. We can create a method to get the wiimote orientation `orient_t WiimoteConnector::getWiimoteOrient()` and then handle this data inside of our main `::execute()`.

<details><summary>Click to show solution</summary>
<p>

```cpp
orient_t wiimoteOrientation = _myWiimote->getWiimoteOrient();

float orientToArray[5];
orientToArray[0] = wiimoteOrientation.roll;
orientToArray[1] = wiimoteOrientation.a_roll;
orientToArray[2] = wiimoteOrientation.pitch;
orientToArray[3] = wiimoteOrientation.a_pitch;
orientToArray[4] = wiimoteOrientation.yaw;

for (int i = 0; i < output->numChannels; i++)
{
    for (int j = 0; j < output->numSamples; j++)
    {
        output->channels[i][j] = float(orientToArray[i]);
    }
}
```

</p>
</details>

### Finishing up the object
When your object is ready for production, make sure you modified the object settings in dll function `FillCHOPPluginInfo`. After that, there is a great guide in the TouchDesigner Documention on [importing your Custom OPs](https://docs.derivative.ca/Custom_Operators#Using_Custom_OPs_-_Plugin_Folder_Locations). To summarize quickly :
1. Compile your DLL project in `Release` mode.
2. Navigate to the `Release` folder and copy the created DLL.
3. Navigate to `C:/Users/<username>/Documents/Derivative/Plugins` and paste your DLL (you might need to create the folder)
4. Copy any necessary DLL to this folder (in our case we will need to copy wiiuse.dll)
5. Start TouchDesigner and enjoy!

### Improvements we can make to our current custom CHOP
Here is a small list of stuff we could do to improve our current object :
* Handle the wiimote dropping
* Handle different type of buttons and gesture
* Put the connection logic inside its own thread

## Closing remark
I hope you had fun in the workshop. Hopefully, you will get some time to process a big chunk of informations like this. One thing is sure, if you have any questions coming up in the next weeks, do not hesitate to poke me on facebook or linkedin or via email and I will be glad to help you. The first detector you will integrate is probably the hardest, after that it starts to get easier. Have a great TD summit!
