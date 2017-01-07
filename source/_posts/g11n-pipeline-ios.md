---
title: Globalization Pipeline for iOS
tags: globalizationpipeline
s: g11n-pipeline-ios
date: 2017-01-06 17:59:34
---


Today we just tagged v1.0 of the [Globalization Pipeline SDK for iOS](https://github.com/IBM-Bluemix/gp-ios-client). What can an iOS client do? Well, let’s build a simple app and find out.

Starting Out
==

First, I’ll launch XCode 8 and create a new workspace.

While that is launching, I’ll warn you that your author is only a recent graduate of the Swift playground, who once deployed some toy apps to a then-new iPhone 3GS. So, it’s been a while. Any suggestions for improvement are welcome. The actual SDK, however, was a team effort.

Today’s app will be a color mixer, to help artists mix their colors. You know, red and blue makes purple, and so on.

I will name the workspace `gp-ios-color-mixer`, and create a new single view app called `GP Color Mixer`.  To simplify things, for now, I disable the checkbox “automatically manage signing.”

{% asset_img 01_singleview.png %}

I want to include the new [SDK](https://github.com/IBM-Bluemix/gp-ios-client). I’ll use [Carthage](https://github.com/Carthage/Carthage) to install it. Since I already have [Homebrew](https://brew.sh) installed, I only need to do


    $ brew install carthage

Now I need a `Cartfile` that mentions the SDK. So I create one at the same level as my XCode project, containing:

    github "IBM-Bluemix/gp-ios-client"

Following the Carthage instructions, I next run

    $ carthage update

which results in 

    *** Fetching gp-ios-client
    *** Checking out gp-ios-client at "v1.0"
    *** xcodebuild output can be found in /var/folders/j9/yn_32djn36x4d4c2mvcr1kgm0000gn/T/carthage-xcodebuild.p2nKN2.log
    *** Building scheme "GPSDK" in TestFramework.xcworkspace

So far so good.  Looking in the Finder, I now have `GPSDK.framework` right where I expect. 

{% asset_img 02_framework.png %}

I’ll add it under “Linked frameworks and Libraries”.

{% asset_img 03_linked.png %}

We also need to make sure the framework is available at runtime. To do that, we add a build phase with a one-line script: `/usr/local/bin/carthage copy-frameworks
` with a single input file -  `$(SRCROOT)/Carthage/Build/iOS/GPSDK.framework`

{% asset_img 04_buildphase.png %}

Will it build? I add this to the top of my generated `ViewController.swift`:

```swift
    import GPSDK
```

I mentioned turning off code signing, but I still ran into some odd warnings:

```
A shell task (/usr/bin/xcrun codesign --force --sign - --preserve-metadata=identifier,entitlements "/Users/srl/Library/Developer/Xcode/DerivedData/gp-ios-color-mixer-evyxcmilwuakdmdvxqqpmmnzisnn/Build/Products/Debug-iphonesimulator/GP Color Mixer.app/Frameworks/GPSDK.framework") failed with exit code 1:
/Users/srl/Library/Developer/Xcode/DerivedData/gp-ios-color-mixer-evyxcmilwuakdmdvxqqpmmnzisnn/Build/Products/Debug-iphonesimulator/GP Color Mixer.app/Frameworks/GPSDK.framework: replacing existing signature
/Users/srl/Library/Developer/Xcode/DerivedData/gp-ios-color-mixer-evyxcmilwuakdmdvxqqpmmnzisnn/Build/Products/Debug-iphonesimulator/GP Color Mixer.app/Frameworks/GPSDK.framework: resource fork, Finder information, or similar detritus not allowed

Command /bin/sh failed with exit code 1
```

Following [QA1940](https://developer.apple.com/library/content/qa/qa1940/_index.html) I was able to make some progress by running `xattr -cr './Carthage/Build/iOS/GPSDK.framework'`.  Now, ⌘R Run rewards me with a blank app window and no errors. Let’s write some code!

Applying myself to the App
==

By _code_, of course, I mean a trip to the storyboard. Let's add a launch icon, because we can.

Now, I add some static fields, two picker views (for the input colors), and a button for action.

{% asset_img 05_controls.png Starting to look like an app… %}


I wrote `Color.swift` to handle the color mixing. It will only support mixing from three of the primary colors - Red, Yellow, Blue. Any other mixing turns into muddy brown. Playground tested, ready to go.

```swift
enum Color : Int {
    case red = 0, orange, yellow, green, blue, purple, muddy;
    // r+y = o, y+b = g, b+r = p
    func simpleDescription() -> String {
    switch self {
        case .red:      return "red"
        case .orange:   return "orange"
        case .yellow:   return "yellow"
        case .green:    return "green"
        case .blue:     return "blue"
        case .purple:   return "purple"
        case .muddy:    return "muddy brown" // use this if we don't know how to mix a color
        // should be exhaustive
        }
    }

    /**
    * Mix the colors, return the result
    */
    func mix( with: Color ) -> Color {
    if( self == .muddy || with == .muddy ) {
        return .muddy // anything + mud = mud
    }
    if( with == self ) {
        return self  // identity!
    }
    switch self {
        case .red:
            switch with {
            case .yellow:   return .orange
            case .blue:     return .purple
            default:        return .muddy
            }
        case .yellow:
            switch with {
            case .red:      return .orange
            case .blue:     return .green
            default:        return .muddy
            }
        case .blue:
            switch with {
            case .red:      return .purple
            case .yellow:   return .green
            default: return .muddy
            }
        default: return .muddy
        }
    }
}
```

Time to wire it up. We create `IBOutlets` for each of the items. And, I’ll clear the result label just to verify that things are wired up. It runs OK, good.

{% asset_img 06_wired.png Wired for sound %}

Now, let’s set up the delegate stuff so that we can get the list of colors showing.

```swift

class ViewController: UIViewController, UIPickerViewDelegate, UIPickerViewDataSource {

// …

// pickerview stuff
func numberOfComponents(in pickerView: UIPickerView) -> Int {
return 1;
}

func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
return 3;
}

let primaryColors = [ Color.red, Color.blue, Color.yellow ]

func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
return primaryColors[row].simpleDescription()
}
```


Hey, just a little more code and we’re feature complete!

```swift
@IBAction func doMix(_ sender: Any) {
let color1 = primaryColors[mixOne.selectedRow(inComponent: 0)]
let color2 = primaryColors[mixTwo.selectedRow(inComponent: 0)]

let newColor = color1.mix(with: color2)

resultLabel.text = newColor.simpleDescription()
}
```

{% asset_img 07_dcut.png Works in English… Ship it (just kidding) %}


At least, feature complete in English.

I’ll next take stock of the resource strings we need to have translated, so that we can run them through the Globalization Pipeline. I’ll call this `gp-color-mixer.json` 

```json
{
    "red": "red",
    "orange": "orange",
    "yellow": "yellow",
    "green": "green",
    "blue": "blue",
    "purple": "purple",
    "muddy brown": "muddy brown",
    "title": "Color Mixer",
    "mix": "Mix"
}
```

Mixing the Blue
===

Time to fire up [Bluemix](https://bluemix.net). We are going to basically follow the [Globalization Pipeline Quick Start Guide](https://github.com/IBM-Bluemix/gp-common#quick-start-guide) for the this part, which I will refer to.

{% asset_img 10_service.png %}

First, I [create an instance](https://github.com/IBM-Bluemix/gp-common#1-create-a-new-globalization-pipeline-service-instance) of the Globalization Pipeline. The name you give the instance doesn’t matter here.

{% asset_img 11_instance.png %}

Now I create a bundle named `gp-color-mixer`. This name _does_ matter, as our iOS app will use it to access the content.

{% asset_img 12_bundle.png %}

I’ll Upload the `gp-color-mixer.json` file above as the source English content, choosing JSON format for the upload. I pick a few languages for the target.

If I view the bundle, I can see our strings there, as well as translated versions.

{% asset_img 13_español.png %}

The Globalization Pipeline offers this web UI to manage content, as well as powerful REST APIs for managing the translation workflow. I need to grant access to the iOS app so that it can _read_ but not _modify_ the translations.  So, switching over to the _API Users_ tab…

{% asset_img 14_api.png %}

The result of creating the API user is that some access information is shown, something like the following:

    API User ID: 5726d656c6f6e7761746572
    Password: aHVudGVyNDIK
    Instance ID: 77617465726d656c6f6e77617465726d
    URL: https://something.something.bluemix.net/something/something

I take these and plug them into a new swift file named `ReaderCredentials.swift` like so: (this is a variant of [ReaderCredentials-SAMPLE.swift](https://github.com/IBM-Bluemix/gp-ios-client/blob/master/TestFramework/TestSDK/TestSDK/ReaderCredentials-SAMPLE.swift) in the SDK’s repo)

```swift
struct ReaderCredentials {
    static let userId       = "5726d656c6f6e7761746572";
    static let password     = "aHVudGVyNDIK";
    static let instanceId   = "77617465726d656c6f6e77617465726d";
    static let url          = "https://something.something.bluemix.net/something/something";
    static let bundleId     = "gp-color-mixer";
}
```

(Now, after putting my _actual_ credentials in, and a brief offscreen struggle with `.gitignore`, I move on…)

Putting it all together
===

I’m almost done.

First, in the `ViewController.swift`, we initialize the GP service and start setting up a few UI items:

```swift
    let gp = GPService()

    func get(key: String) -> String {
        return gp.localizedString(key, nil)
    }
    
    func get(color: Color) -> String {
        return get(key: color.simpleDescription())
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()

        resultLabel.text = "Loading…"
        do {
            try gp.initService(url: ReaderCredentials.url,
                instanceId: ReaderCredentials.instanceId,
                bundleId:   ReaderCredentials.bundleId,
                userId:     ReaderCredentials.userId,
                password:   ReaderCredentials.password,
                
                languageId:nil,
                alwaysLoadFromServer: false,
                expireAfter: 0)

            // set up strings
            titleLabel.text = get(key: "title")
            mixButton.setTitle(get(key: "mix"), for: UIControlState.normal)
            mixButton.titleLabel?.text = get(key: "mix")
            resultLabel.text = "" // clear this
        } catch GPService.GPError.languageNotSupported {
            resultLabel.text = ("This language is not supported...")
        } catch GPService.GPError.requestServerError(let errorDescription) {
            resultLabel.text = ("Request server error: " + errorDescription)
        } catch GPService.GPError.HTTPError(let statusCode) {
            resultLabel.text = ("Request server error: HTTP \(statusCode)")
        } catch {
            resultLabel.text = ("Other error")
        }
    }
```

Here we set up the service with our credentials. Then, we use our new `get(key: )` function to set the title and mix button’s label.

There is also a `get(color: )` variant that will translate one of our `Color` objects. So we use that for the actual mixing function:

```swift
    @IBAction func doMix(_ sender: Any) {
        …
        resultLabel.text = get(color: newColor)
```

Similarly, we can get the `UIPickerView` to use localized color names by using this same function:

```swift
    func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        return get(color: primaryColors[row])
    }
```

Looks good!


{% asset_img 20_bueno.png Now we can ship it… to the world! %}


Conclusion
===

The iOS app will pick up changes if the translated content changes on the server. We could experiment with adding or removing languages, or updating translated keys.

You can find the source code at [https://github.com/srl295/gp-ios-color-mixer](https://github.com/srl295/gp-ios-color-mixer).

Let me know if this works for you. This is my first post, and as I mentioned first app, in [Swift](https://swift.org) so that’s a milestone. And, do let me know if`^H^H` what can be done to improve the sample app.

Thanks! Now go and make it global.
