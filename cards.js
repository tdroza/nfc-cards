var app = angular.module('cardsApp', []);

app.controller('cardsController', function($scope, $http, $location) {
  $http.get("./data.json")
  .then(function(response) {
      $scope.show=$location.search().show||response.data.length;
      console.log('Scope.show: ' + $scope.show);
      $scope.cardsData = response.data;
  });
});