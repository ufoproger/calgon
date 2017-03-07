<?php

abstract class Solver
{
	const INFINITY = 1000000;

	public $steps = [];
	public $name = "";

	public function pushStep($d, $h, $x = null)
	{
		$step = [];

		foreach ($d as &$value)
			if ($value === self::INFINITY)
				$value = '&infin;';

		foreach ($h as &$value)
			$value++;

		foreach ($x as &$value)
			$value = (int)$value;

		$step['D[i]'] = $d;
		$step['H[i]'] = $h;

		if ($x)
			$step['X[i]'] = $x;

		$this->steps[] = $step;
	}

	static public function prepareMatrixA($a)
	{
		foreach ($a as &$row)
			foreach ($row as &$element)
			{
				if (empty($element))
					$element = self::INFINITY;

				$element = intVal($element);
			}

		return $a;
	}

	abstract public function calc($a, $from, $to);
}

class SolverDijkstra extends Solver
{
	public $name = "Алгоритм Дейкстры";

	public function calc($a, $from, $to)
	{
		$size = count($a);
		$d = array_fill(0, $size, self::INFINITY);
		$h = array_fill(0, $size, -1);
		$x = array_fill(0, $size, false);

		$d[$from] = 0;
		$x[$from] = true;
		$previous = $from;

		$this->pushStep($d, $h, $x);

		while ($previous != $to)
		{
			foreach ($a[$previous] as $j => $weight)
			{
				if ($x[$j])
					continue;

				if ($d[$j] > $d[$previous] + $weight)
				{
					$d[$j] = $d[$previous] + $weight;
					$h[$j] = $previous;
				}
			}

			$min = null;

			foreach ($d as $i => $value)
			{
				if ($x[$i])
					continue;

				if (is_null($min) || $d[$min] > $value)
					$min = $i;
			}

			$x[$min] = true;
			$previous = $min;

			$this->pushStep($d, $h, $x);
		}
	}
}

class SolverFloyd extends Solver
{
	public $name = "Алгоритм Флойда";

	public function calc($a, $from, $to)
	{
		echo 'dfgdf';	
	}
}

class SolverFactory
{
	private function __construct() {}

	public static function create($algo)
	{
		switch ($algo)
		{
			case 'dijkstra':
				return new SolverDijkstra;
	
			case 'floyd':
				return new SolverFloyd;
		}

		return null;
	}
}