// swift-tools-version:5.3
// The swift-tools-version declares the minimum version of Swift required to build this package.
import PackageDescription
let package = Package(
    name: "TestARM2D",
    dependencies: [
        // Dependencies declare other packages that this package depends on.
        .package(url: "https://github.com/madmachineio/SwiftIO.git", .upToNextMajor(from: "0.0.4")),
        .package(url: "https://github.com/madmachineio/MadBoards.git", .upToNextMajor(from: "0.0.3")),
        .package(url: "https://github.com/madmachineio/MadDrivers.git", .upToNextMajor(from: "0.0.8")),
        //.package(url: "https://github.com/madmachineio/MadARM2D.git", .branch("main")),
        .package(path: "/mnt/e/westz/swiftio/src/MadARM2D"),
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages this package depends on.
        .target(
            name: "TestARM2D",
            dependencies: [
                "SwiftIO",
                "MadBoards",
                "MadARM2D",
                // use specific library would speed up the compile procedure
                .product(name: "MadDrivers", package: "MadDrivers")
            ]),
        .testTarget(
            name: "TestARM2DTests",
            dependencies: ["TestARM2D"]),
    ]
)
