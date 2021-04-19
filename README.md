<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/atuatohu/Plant-o-Matic">
    <img src="https://github.com/atuatohu/Plant-o-Matic/blob/main/Materials/product_logo.PNG" alt="Logo" width="235" height="235">
  </a>

  <h2 align="center">Plant-o-Matic</h2>

  <p align="center">
    Automated plant care and monitoring system using Raspberry Pi Model 3 B+
    <br />
    <a href="https://github.com/atuatohu/Plant-o-Matic"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/atuatohu/Plant-o-Matic">View Demo</a>
    ·
    <a href="https://github.com/atuatohu/Plant-o-Matic/issues">Report Bug or Request a Feature</a>
  </p>
  <p align="center">
    <img src="https://github.com/atuatohu/Plant-o-Matic/blob/main/Materials/svg_logos/YouTube.svg" width="45">&nbsp;&nbsp;&nbsp;
    <img src="https://github.com/atuatohu/Plant-o-Matic/blob/main/Materials/svg_logos/Twitter.svg" width="45">&nbsp;&nbsp;&nbsp;
    <img src="https://github.com/atuatohu/Plant-o-Matic/blob/main/Materials/svg_logos/Instagram.svg" width="45">&nbsp;&nbsp;&nbsp;
  </p>
</p>

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about">About</a></li>
    <li><a href="#hardware">Hardware</a></li>
    <li><a href="#how-to-start">How to Start</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#references">References</a></li>
  </ol>
</details>

## About

**Plant-o-Matic** is an automated plant care and monitoring system implemented on Raspberry Pi Model 3 B+ with the help of C++, nginx and FastCGI.

For some people plants are parts of their families and taking care of their plant is as crucial as for other people to take care of their pets. For others, they are something they keep forgetting about because of the busy schedule or heaps of thoughts.

If you belong to the latter category, this project is for you! With it's help, it is possible not just to grow a plant, but also monitor it's state and other related data.

## Hardware

* [Raspberry Pi 3 Model B+](https://www.raspberrypi.org/products/raspberry-pi-3-model-b-plus/)

* [32GB MicroSD](https://www.amazon.co.uk/SanDisk-microSDHC-Adapter-Performance-SDSQUA4-032G-GN6MA/dp/B08GY9NYRM/ref=sr_1_3?dchild=1&keywords=micro+sd&qid=1618756194&sr=8-3)

* [MCP3008 ADC](https://thepihut.com/products/adafruit-mcp3008-8-channel-10-bit-adc-with-spi-interface)

<a href="https://thepihut.com/products/adafruit-mcp3008-8-channel-10-bit-adc-with-spi-interface">
  &nbsp;&nbsp;&nbsp;<img src="https://cdn.shopify.com/s/files/1/0176/3274/products/100460_400x.jpg?v=1540884052" alt="mcp3008" width="190" height="190">
</a>

* [Capacitive Soil Moisture Sensor v1.2](https://thepihut.com/products/capacitive-soil-moisture-sensor)

<a href="https://thepihut.com/products/capacitive-soil-moisture-sensor">
  &nbsp;&nbsp;&nbsp;<img src="https://cdn.shopify.com/s/files/1/0176/3274/products/0d667283-1ff8-47af-acb3-e36f83d244a5_400x.jpg?v=1592639176" alt="mcp3008" width="190" height="190">
</a>

* [HC-SR04 Ultrasonic sensor ](https://thepihut.com/products/ultrasonic-distance-sensor-hcsr04)

<a href="https://thepihut.com/products/ultrasonic-distance-sensor-hcsr04">
  &nbsp;&nbsp;&nbsp;<img src="https://cdn.shopify.com/s/files/1/0176/3274/products/100284_400x.jpg?v=1540887570" alt="hc-sr04" width="190" height="190">
</a>

* [DHT22 Temperature and Humidity Sensor](https://www.amazon.co.uk/gp/product/B01N6PB489/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)

<a href="https://www.amazon.co.uk/gp/product/B01N6PB489/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1">
  &nbsp;&nbsp;&nbsp;<img src="https://images-na.ssl-images-amazon.com/images/I/71JDpqTmLqL._AC_SL1500_.jpg" alt="dht22" width="170" height="150">
</a>

* [6-12V Water pump](https://www.amazon.co.uk/gp/product/B0744FWNFR/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1)

<a href="https://www.amazon.co.uk/gp/product/B0744FWNFR/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1">
  &nbsp;&nbsp;&nbsp;<img src="https://images-na.ssl-images-amazon.com/images/I/61sqqq%2BlR-L._AC_SL1500_.jpg" alt="water pump" width="190" height="210">
</a>

* [5 to 12 V Isolated DC/DC Converter](https://www.mouser.co.uk/ProductDetail/TRACO-Power/TRA-1-0512?qs=ckJk83FOD0VZ5q6Ozn6vsw==&utm_source=OEMSECRETS&utm_medium=aggregator&utm_campaign=TRA+1-0512&utm_term=TRA+10512&utm_content=TRACO+Power)

<a href="https://www.mouser.co.uk/ProductDetail/TRACO-Power/TRA-1-0512?qs=ckJk83FOD0VZ5q6Ozn6vsw==&utm_source=OEMSECRETS&utm_medium=aggregator&utm_campaign=TRA+1-0512&utm_term=TRA+10512&utm_content=TRACO+Power">
  &nbsp;&nbsp;&nbsp;<img src="https://www.tracopower.com/sites/default/files/styles/product_detail/public/products/pictures/tra1_mainpicture.jpeg?itok=h67IPxCM" alt="tra 1-0512" width="190" height="190">
</a>

## How to Start

Before downloading the project, check if the following prerequisites are present on your Raspberry Pi:

* [wiringPi](http://wiringpi.com/)
* [nginx](https://www.nginx.com/)
* [cmake](https://cmake.org/)
* [git](https://git-scm.com/)
* SPI interface Enabled

If not, please visit our [wiki page](https://github.com/atuatohu/Plant-o-Matic/wiki/Installation-guide) for detailed instructions. 

<!-- ROADMAP -->
## Roadmap

See the [open issues](https://github.com/atuatohu/Plant-o-Matic/issues) for a list of proposed features (and known issues).

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<!-- LICENSE -->
## License

Distributed under the MIT License. See [LICENSE](https://github.com/atuatohu/Plant-o-Matic/blob/main/LICENSE) for more information.

<!-- CONTACT -->
## Contact

[Aigerim Abdurakhmanova](https://github.com/Aigerim98)

[Adil Shakeel](https://github.com/2624480S)

[Medet Kassymov](https://github.com/atuatohu)


Project Link: [https://github.com/atuatohu/Plant-o-Matic](https://github.com/atuatohu/Plant-o-Matic)

<!-- REFERENCES -->
## References
* [README Template](https://github.com/othneildrew/Best-README-Template) 

