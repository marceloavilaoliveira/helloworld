# sandbox/helloworld

Este é um repositório de testes.

## BUILD

* Clean:

```bash
$ mvn clean
```

* Compile:

```bash
$ mvn compile
```

* Compile + Test:

```bash
$ mvn test
```

* Compile + Test + Package:

```bash
$ mvn package
```

* Clean + Compile + Test + Package:

```bash
$ mvn clean package
```

## SONARQUBE

* Download and install SonarScanner:

[SonarQube/SonarScanner latest documentation](https://docs.sonarqube.org/latest/analysis/scan/sonarscanner/)

* Copy the sonar-scanner.properties global settings file to the SonarScanner location

```bash
$ cp sonar-scanner.properties <SONARSCANNER-PATH>/conf/.
```

* Run cppcheck analysis (C code):

```bash
$ mkdir -p target/cppcheck-reports
$ cppcheck --xml --enable=all src/main/. 2> target/cppcheck-reports/cppcheck-report.xml
```

* Run analysis:

```bash
$ sonar-scanner -Dsonar.branch.name=<YOUR-BRANCH-NAME>
```

## EXECUTION

* Execute:

```bash
$ java -jar target/helloworld-<VERSION$-SNAPSHOT.jar [<parameters>]
```
