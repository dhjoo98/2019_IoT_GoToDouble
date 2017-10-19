<template>
  <chart :options="chartOption" />
</template>

<script>
  import ECharts from 'vue-echarts/components/ECharts.vue'
  import 'echarts'    

  export default {
    name: 'HelloWorld',
    props: ['chartData'],
    data() {
      return { 
        
      }
    },
    mounted() {},
    methods: {},
    computed: {
      chartOption() {
        const option = {
          tooltip: {
            trigger: 'axis',
            axisPointer: {
              type: 'cross'
            }
          },
          xAxis: {
            type: 'category',
            boundaryGap: false,
            data: this.chartData.times, 
          },
          yAxis: {
            type: 'value',
            axisLabel: {
              formatter: '{value}'
            },
            axisPointer: {
              snap: true
            }
          },
          visualMap: [{
            show: false,
            type: 'continuous',
            seriesIndex: 0,
            min: 0,
            max: 400
          }],
          color: ['#79b4b0'],
          background: ['#fff'],
          series: [{
            name: '심박수',
            type: 'line',
            smooth: true,
            data: this.chartData.heartData,
            markArea: {
              data: [
                [{ 
                  yAxis:  this.chartData.minData
                }, {
                  yAxis: this.chartData.maxData
                }]
              ]
            },
            markPoint: {
              data: [{
                  type: 'max',
                  name: '최대'
                },
                {
                  type: 'min',
                  name: '최소'
                }
              ]
            },
            markLine: {
              data: [{
                type: 'average',
                name: '평균'
              }]
            }
          }]
        };
        return option;
      }
    },
    components: {
      chart: ECharts
    }
  }

</script>
 
<style> 

  .echarts {
    height: 200px !important;
    width: 400px !important;
    
  }
 
</style>
