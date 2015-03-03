/**
 * @file
 * Client-side script controlling the display of the live statistics.
 */

(function ($, Drupal) {

  'use strict';

  Drupal.behaviors.election_livestats_page = {

    attach: function (context, settings) {

      // As we are clearly running JavaScript, prepare the scene for the live statistics.
      var $statistics = $("<div id='statistics'>");
      $('#noscript-statistics').after($statistics).remove();

      // Run through the initial data to create the charts for each metric.
      initial_data.forEach(function(metric){

        var $section = $("<div>").attr('id', 'section-' + metric.name).appendTo($statistics);
        $("<h2>").text(metric.label).appendTo($section);

        var $chart = $("<div>").addClass("chart").appendTo($section);

        if(metric.type === "single" || metric.type === "grouped") {

          $chart.highcharts({
            credits: false,
            title: false,
            legend: {
              enabled: false
            },
            chart: {
              backgroundColor: 'transparent',
              type: 'bar'
            },
            plotOptions: {
              series: {
                groupPadding: 0
              },
              bar: {
                grouping: false,
                borderWidth: 0
              }
            },
            tooltip: {
              formatter: function () {
                return this.point.label;
              }
            },
            xAxis: {
              gridLineWidth: 0,
              minorGridLineWidth: 0,
              lineColor: 'transparent',
              tickLength: 0,
              tickInterval: 1,
              type: 'category',
              labels: {
                style: {
                  fontSize: '14px',
                }
              }
            },
            yAxis: {
              gridLineWidth: 0,
              minorGridLineWidth: 0,
              lineColor: 'transparent',
              labels: {
                enabled: false,
              },
              title: false,
              min: 0,
              max: 1,
            },
            series: [{
              color: '#dddddd',
              enableMouseTracking: false,
              data: []
            }, {
              color: '#582c83',
              data: []
            }]
          });

        }

        update(metric);

      });

      // Set up socket for updating data for this election.
      // `socket_io_url` is set through inline JavaScript included in the
      // page callback.
      try {
        var socket = io.connect(socket_io_url);
        socket.emit('subscribe', election_id);

        // Respond to update packets form the websocket.
        socket.on('update', function(data) {

          update(data);

        });
      } catch (e) {
        $statistics.before('<div class="messages error">The live statistics server is currently not running</b>. Please refresh this page for updates!</div>');
      }

      function update(metric) {
        var $section = $('#section-' + metric.name);

        if(metric.type === "single") {
          var height = 80;

          var value = metric.value;
          var total = metric.total;

          // Avoid NaN if the total possible is zero.
          var ratio = (total === 0) ? 0 : value / total;

          var data = [{
            name: ' ',
            label: value + '/' + total + ' (' + Math.round(ratio * 100) + '%)',
            y: ratio
          }];

          var chart = $('.chart', $section).highcharts()
          chart.series[0].setData([{ y: 1 }]);
          chart.series[1].setData(data);

          chart.setSize(chart.chartWidth, height);
        } else if(metric.type === "grouped") {
          var height = 40 * Object.keys(metric.total).length;

          var data = [];
          var bg = [];

          for(var id in metric.total) {

            var value = metric.value[id];
            var total = metric.total[id].value;

            // Avoid NaN if the total possible is zero.
            var ratio = (total === 0) ? 0 : value / total;

            data.push({
              name: metric.total[id].label,
              label: value + '/' + total + ' (' + Math.round(ratio * 100) + '%)',
              y: ratio
            });
            bg.push({ y: 1 });
          }

          data.sort(function(a, b) {
            if(a.y < b.y) {
              return 1;
            }else if(a.y > b.y) {
              return -1;
            }else{
              return 0;
            }
          });

          var chart = $('.chart', $section).highcharts();
          chart.series[0].setData(bg);
          chart.series[1].setData(data);
          chart.setSize(chart.chartWidth, height);
        } else if(metric.type === "count") {
          $section.children().eq(1).text(metric.value);
        }

      }

    }

  };

}(jQuery, Drupal));
