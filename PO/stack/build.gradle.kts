import org.gradle.kotlin.dsl.run as runApplication

val enablePreview = "--enable-preview"

plugins {
    java
    application
}

group = "pl.edu.mimuw"
version = "1.0-SNAPSHOT"

repositories {
    mavenCentral()
}

application {
    mainClass.set("pl.edu.mimuw.Main")
}

tasks.compileJava {
    options.compilerArgs.add(enablePreview)
}

tasks.runApplication {
    jvmArgs(enablePreview)
    standardInput = System.`in`
}
