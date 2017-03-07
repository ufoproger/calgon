<?php
$examples = require_once('examples.php');
?>
<!DOCTYPE html>
<html lang="en">
  <head>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
	<title>Нахождение кратчайшего пути в графе</title>
	<link href="vendor/twbs/bootstrap/dist/css/bootstrap.min.css" rel="stylesheet">
	<link href="style.css" rel="stylesheet">

	<!--[if lt IE 9]>
		<script src="https://cdnjs.cloudflare.com/ajax/libs/html5shiv/3.7.3/html5shiv.js"></script>
	<![endif]-->

	<script type="text/javascript">
		<? printf("var examples = %s;", json_encode($examples)) ?>
	</script>
  </head>

  <body>

	<div class="container">

	  <div class="starter-template">
		<h3>Нахождение кратчайшего пути в графе</h3>
		<p class="lead">алгоритм Дейкстры, алгоритм Флойда</p>

		<form method="GET" action="solution.php">
			<div class="row justify-content-center">
				<? foreach (['size' => 'Размер графа', 'from' => 'Исходная вершина', 'to' => 'Конечная вершина'] as $name => $title): ?>
					<div class="form-group col-md-3">
						<label for="graph-<?= $name ?>"><?= $title ?></label>
						<select class="form-control" name="<?= $name ?>" id="graph-<?= $name ?>">
							<? for ($i = 1; $i <= 10; $i++): ?>
								<option value="<?= $i ?>"><?= $i ?></option>
							<? endfor; ?>
						</select>
					</div>
				<? endforeach; ?>
				<div class="form-group col-md-3">
					<label for="graph-<?= $name ?>">Алгоритм</label>
					<select class="form-control" name="algo" id="graph-algo">
						<option value="dijkstra">Дейкстры</option>
						<option value="floyd">Флойда</option>
					</select>
				</div>
			</div>
		
			<table class="table" id="graph-matrix"></table>
			<button type="submit" class="btn btn-primary">Решить</button>
		</form>
		<br/>

		<h4>Примеры:</h4>
		<? foreach ($examples as $index => $example): ?>
			<p>
				<a href="#" class="edit-example" data-example-index="<?= $index ?>"><?= $example['title'] ?></a>
			</p>
		<? endforeach; ?>
	</div>
	
	<script src="js/jquery-3.1.1.min.js"></script>
	<script src="js/tether.min.js"></script>
	<script src="index.js"></script>
	<script src="vendor/twbs/bootstrap/dist/js/bootstrap.min.js"></script>
	<script src="js/ie10-viewport-bug-workaround.js"></script>
  </body>
</html>