// swift-tools-version:5.3
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let cSettings: [CSetting] = [
    .headerSearchPath("../arm-2d-header"),
    .headerSearchPath("../CMSIS/Core/Include"),
    .headerSearchPath("../CMSIS/DSP/Include"),
]

let package = Package(
    name: "MadARM2D",
    products: [
        // Products define the executables and libraries a package produces, and make them visible to other packages.
        .library(
            name: "MadARM2D",
            targets: [
                "arm-2d",
                "arm-2d-wapper"]),
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages this package depends on.
        .target(
            name: "arm-2d",
            cSettings: cSettings),
        .target(
            name: "arm-2d-wapper",
            cSettings: cSettings)
    ]
)
