var app = angular.module('cardsApp', []);
app.controller('cardsController', function($scope, $http) {
  $http.get("./data.json")
  .then(function(response) {
      console.log(response.data)
      $scope.cardsData = response.data;
  });
});